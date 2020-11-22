#include "widget_home.h"
#include "soma.h"
#include "wt/wt.h"

widget_home::widget_home() : wcontainer("home")
{
	// load css + js lib if not loaded
	S->application()->require("js/function.js");
	S->application()->require("js/image.js");
	S->application()->require("js/scrollbar.js");
	S->application()->useStyleSheet("css/image.css");
	S->application()->useStyleSheet("css/scrollbar.css");
	this->setCanReceiveFocus(true); // allow focus, so we can remove focus from search if needed

	// css animations
	bindString("css_animations", (global::css_animations) ? "" : "no-animation");

	setStyleClass("widget_home");
	search = bindNew<widget_search>("widget_search");
	string ls_data = system::shellexec("cd /dalaran/brinespell/data && find . -name '*' -type f | grep -v '/campaign/' | sed -e 's,^\\./,,'");
	search->set_data(explode("\n", ls_data));
	search->edit_search->setFocus(true);

	dynamic = bindNew<widget_dynamic>("widget_dynamic");
	audio = bindNew<widget_audio>("widget_audio");
	dices = bindNew<widget_dice>("widget_dice");
	chat = bindNew<widget_chat>("widget_chat");
	character = bindNew<widget_character>("widget_character");
	party = bindNew<widget_party>("widget_party");

	/****    drag & drop    ****/

	acceptDrops("player_token");

	/****    signal binding    ****/

	// search
	search->on_select_event.connect([&](string filename)
	{
		search_master_open(filename);
	});
	search->remove_focus_event.connect([&](){ this->setFocus(true); });

	// audio
	audio->on_switch_pause_event.connect([&](bool paused)
	{
		// only broadcast if we are the game master
		if (!S->p_shadow->game_master) return;
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
	chat->remove_focus_event.connect([&](){ this->setFocus(true); });

	// misc
	S->globalKeyPressed().connect(this, &widget_home::global_key_pressed);

	// character
	character->details_hp->hit_point_event.connect([&](int percent, string helper)
	{
		broadcast::all(&widget_home::update_ally_hp, (int)S->p_player.id(), percent, helper);
	});

	// party
	party->impersonate_event.connect([&]()
	{
		character->update_character();
	});

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
	if (e.key() == Key::Enter && e.modifiers() == KeyboardModifier::Shift)
	{
		chat->chat_input->setFocus(true);
	}
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
	// special cases
	if (filename == "brinespell/reload")
	{
		string ls_data = system::shellexec("cd /dalaran/brinespell/data && find . -name '*' -type f | grep -v '/campaign/' | sed -e 's,^\\./,,'");
		search->set_data(explode("\n", ls_data));
		search->edit_search->setFocus(true);
		return;
	}

	string ext = file::extension(filename);
	if (
		ext == "mp4"
		|| ext == "mp3"
		|| ext == "webm"
		|| ext == "wav"
		|| ext == "ogg")
	{
		// only broadcast if we are the game master
		if (!S->p_shadow->game_master) return;
		broadcast::all(&widget_home::change_audio_track, "data/" + filename);
	}
	else if (
		ext == "png"
		|| ext == "jpg"
		|| ext == "jpeg")
	{

		// open image and, open the same image via broadcast::other with the same id
		string id = dynamic->open_image("data/" + filename);

		// only broadcast if we are the game master
		// TODO: tmp, allow everyone to share image
		/* if (!S->p_shadow->game_master) return; */

		broadcast::others(&widget_dynamic::open_shared_image, "data/" + filename, id);
	}
}

void widget_home::dropEvent(WDropEvent e)
{
	// don't handle touch event for now
	if (e.originalEventType() != WDropEvent::OriginalEventType::Mouse) return;

	if (e.mimeType() == "player_token")
	{
		auto token = dynamic_cast<widget_drag_token*>(e.source());
		if (! token) return; // bad cast

		string id = dynamic->open_token_player(token->p_player,
			e.mouseEvent()->window().y,
			e.mouseEvent()->window().x);
	}
}

/****    audio    ****/

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

/****    dice    ****/

void widget_home::throw_dice(string notation, string rand_init)
{
	auto p_soma = soma::application();
	if (!p_soma->view_home) return;

	auto &dices = p_soma->view_home->dices;
	dices->throw_dice_nocallback(notation, rand_init);
}

/****    chat    ****/

void widget_home::chat_message(string message)
{
	auto p_soma = soma::application();
	if (!p_soma->view_home) return;

	auto &chat = p_soma->view_home->chat;
	chat->add_message(message);
}

/****    ally    ****/

void widget_home::update_ally_hp(int player_id, int percent, string helper)
{
	auto p_soma = soma::application();
	if (!p_soma->view_home) return;
	if (!p_soma->view_home->party) return;

	p_soma->view_home->party->update_hit_point(player_id, percent, helper);
}
