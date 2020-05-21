#include "widget_home.h"
#include "soma.h"
#include "wt/wt.h"

widget_home::widget_home() : wcontainer("home")
{
	// load js custom lib if not loaded
	S->application()->require("js/function.js");

	setStyleClass("widget_home");
	search = bindNew<widget_search>("widget_search");
	string ls_data = system::shellexec("cd /dalaran/brinespell/data && find . -name '*' -type f | sed -e 's,^\\./,,'");
	search->set_data(explode("\n", ls_data));
	search->edit_search->setFocus(true);

	audio = bindNew<widget_audio>("widget_audio");
	dices = bindNew<widget_dice>("widget_dice");
	chat = bindNew<widget_chat>("widget_chat");

	/****    signal binding    ****/

	// search
	search->on_select_event.connect([&](string filename)
	{
		search_open(filename);
	});

	search_open("img/crypt.jpg");

	// audio
	audio->on_switch_pause_event.connect([&](bool paused)
	{
		// only broadcast if we are the game master
		if (!S->p_player->game_master) return;
		broadcast::others(&widget_home::switch_pause_audio_track, paused);
	});

	// dice
	dices->throw_dice_event.connect([&](string notation, string rand_init)
	{
		broadcast::others(&widget_home::throw_dice, notation, rand_init);
	});
	dices->dice_results_event.connect([&](string res)
	{
		res = widget_chat::prepare_message(res);
		broadcast::all(&widget_home::chat_message, res);
	});
	dices->dice_secret_results_event.connect([&](string res)
	{
		res = widget_chat::prepare_message(res);
		chat_message(res);
	});

	// chat
	chat->chat_input_event.connect([&](string message)
	{
		broadcast::all(&widget_home::chat_message, message);
	});
	S->globalEnterPressed().connect([&](){ chat->chat_input->setFocus(true); });

	// player join chat info
	string current_time = wt::current_time().toString("HH:mm").toUTF8();
	string message =
		"<span class=\"widget_chat_timestamp\">"
		+ current_time
		+ "</span>"
		"<span class=\"widget_chat_join\">"
		+ S->p_player->name
		+ " joins the session</span>"
		+ "<br />\n";
	broadcast::all(&widget_home::chat_message, message);
}

void widget_home::search_open(string filename)
{
	string ext = file::extension(filename);
	if (
		ext == "mp4"
		|| ext == "mp3"
		|| ext == "webm"
		|| ext == "wav"
		|| ext == "ogg")
	{
		// only broadcast if we are the game master
		if (!S->p_player->game_master) return;
		broadcast::all(&widget_home::change_audio_track, "data/" + filename);
	}
	else if (
		ext == "png"
		|| ext == "jpg"
		|| ext == "jpeg")
	{
		broadcast::all(&widget_home::open_image, "data/" + filename);
	}
}

void widget_home::change_audio_track(string filename)
{
	/* debug_line(filename); */
	auto p_soma = soma::application();
	if (!p_soma->view_home) return;

	auto &audio = p_soma->view_home->audio;

	audio->load_audio(filename);
	audio->play();
}

void widget_home::switch_pause_audio_track(bool paused)
{
	auto p_soma = soma::application();
	if (!p_soma->view_home) return;

	auto &audio = p_soma->view_home->audio;

	if (paused)
	{
		audio->pause();
	}
	else
	{
		audio->play();
	}
}

void widget_home::throw_dice(string notation, string rand_init)
{
	auto p_soma = soma::application();
	if (!p_soma->view_home) return;

	auto &dices = p_soma->view_home->dices;
	dices->throw_dice_nocallback(notation, rand_init);
}

void widget_home::chat_message(string message)
{
	auto p_soma = soma::application();
	if (!p_soma->view_home) return;

	auto &chat = p_soma->view_home->chat;
	chat->add_message(message);
}

void widget_home::open_image(string filename)
{
	auto p_soma = soma::application();
	if (!p_soma->view_home) return;

	p_soma->main_div->addNew<widget_image>(filename);
}
