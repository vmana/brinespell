#include "widget_home.h"
#include "soma.h"

widget_home::widget_home() : wcontainer("home")
{
	// load js custom lib if not loaded
	S->application()->require("js/function.js");

	setStyleClass("widget_home");
	/* this->clicked().connect(broadcast::all(&widget_home::test2, {"aaa"}, {"bbb"})); */

	/* auto player = this->addChild(make_unique<WMediaPlayer>(MediaType::Audio)); */
	search = bindNew<widget_search>("widget_search");
	search->set_data(system::ls("/dalaran/brinespell/data"));
	search->edit_search->setFocus(true);
	/* search->on_select_event.connect([=](string value){ debug_line(value); }); */

	audio = bindNew<widget_audio>("widget_audio");
	dices = bindNew<widget_dice>("widget_dice");
	chat = bindNew<widget_chat>("widget_chat");

	// signal binding
	search->on_select_event.connect([&](string filename){ broadcast::all(&widget_home::change_audio_track, "data/" + filename); });
	dices->dice_results_event.connect([&](string res)
	{
		chat->add_message("Dice results " + res);
	});
}

void widget_home::change_audio_track(string filename)
{
	debug_line(filename);
	auto p_soma = soma::application();
	if (!p_soma->view_home) return;

	auto &audio = p_soma->view_home->audio;

	audio->load_audio(filename);
	audio->mediaplayer->play();
}


