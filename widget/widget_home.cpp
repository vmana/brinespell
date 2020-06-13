#include "widget_home.h"
#include "soma.h"
#include "wt/wt.h"

widget_home::widget_home() : wcontainer("home")
{
	// load js custom lib if not loaded
	S->application()->require("js/function.js");

	setStyleClass("widget_home");
	search = bindNew<widget_search>("widget_search");
	string ls_data = system::shellexec("cd /dalaran/brinespell/data && find . -name '*' -type f | grep -v '/campaign/' | sed -e 's,^\\./,,'");
	search->set_data(explode("\n", ls_data));
	search->edit_search->setFocus(true);

	audio = bindNew<widget_audio>("widget_audio");
	dices = bindNew<widget_dice>("widget_dice");
	chat = bindNew<widget_chat>("widget_chat");
	character = bindNew<widget_character>("widget_character");

	/****    signal binding    ****/

	// search
	search->on_select_event.connect([&](string filename)
	{
		search_master_open(filename);
	});

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
	S->globalKeyPressed().connect(this, &widget_home::global_key_pressed);

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

void widget_home::global_key_pressed(WKeyEvent e)
{
	if (e.key() == Key::Enter)
	{
		search->edit_search->setFocus(true);
	}
	else if (e.key() == Key::Space)
	{
		audio->on_play_pause_click();
	}
}

void widget_home::search_master_open(string filename)
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

		// open image and, open the same image via broadcast::other with the same id
		string id = open_image("data/" + filename);

		// only broadcast if we are the game master
		if (!S->p_player->game_master) return;

		broadcast::others(&widget_home::open_shared_image, "data/" + filename, id);
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

string widget_home::open_image(string filename)
{
	string id = mana::randstring(16);
	auto img = S->main_div->addNew<widget_image>(filename, id);

	// signals binding
	img->on_move_event.connect([=](int top, int left)
	{
		broadcast::others(&widget_home::move_image, id, top, left);
	});
	// needs tuple since wt signal connect only allows 3 parameters max
	img->on_resize_event.connect([=](tuple<int, int, int, int> pos)
	{
		auto &[top, left, width, height] = pos;
		broadcast::others(&widget_home::resize_image, id, top, left, width, height);
	});
	img->on_zoom_event.connect([=](tuple<int, int, int, int> pos)
	{
		auto &[zoom_w, zoom_h, zoom_x, zoom_y] = pos;
		broadcast::others(&widget_home::zoom_image, id, zoom_w, zoom_h, zoom_x, zoom_y);
	});
	img->on_close_event.connect([=]()
	{
		broadcast::others(&widget_home::close_image, id);
	});
	img->on_view_mode_event.connect([=](string mode)
	{
		broadcast::others(&widget_home::switch_mode_image, id, mode);
	});
	img->on_shared_event.connect([=](bool shared)
	{
		broadcast::others(&widget_home::change_image_visibility, id, shared);
	});

	return id;
}

void widget_home::open_shared_image(string filename, string id)
{
	auto p_soma = soma::application();
	if (!p_soma->view_home) return;
	p_soma->main_div->addNew<widget_image>(filename, id, false);
}

void widget_home::move_image(string id, int top, int left)
{
	auto img = search_image(id);
	if (img) img->animate_position(top, left);
}

void widget_home::resize_image(string id, int top, int left, int width, int height)
{
	auto img = search_image(id);
	if (img) img->animate_resize(top, left, width, height);
}

void widget_home::zoom_image(string id, int zoom_w, int zoom_h, int zoom_x, int zoom_y)
{
	auto img = search_image(id);
	if (img) img->animate_zoom(zoom_w, zoom_h, zoom_x, zoom_y);
}

void widget_home::switch_mode_image(string id, string mode)
{
	auto img = search_image(id);
	if (img) img->change_view_mode(mode);
}

void widget_home::change_image_visibility(string id, bool visible)
{
	auto img = search_image(id);
	if (img) img->change_image_visibility(visible);
}

void widget_home::close_image(string id)
{
	auto img = search_image(id);
	if (img) img->close();
}

widget_image* widget_home::search_image(string id)
{
	widget_image *ret = NULL;
	auto p_soma = soma::application();
	if (!p_soma->view_home) return ret;

	// search for a child with this id
	for (auto &child : p_soma->main_div->children())
	{
		if (child->id() == id)
			return (widget_image*)child;
	}
	return ret;
}
