#include "widget_home.h"
#include "soma.h"

using namespace std::placeholders;

widget_home::widget_home() :
	wcontainer("home"),
	signal_dice_results(this, "signal_dice_results")
{
	S->application()->require("js/cannon.min.js");
	S->application()->require("js/three.min.js");
	S->application()->require("js/dice.js");
	setStyleClass("widget_home");

	/* this->clicked().connect(broadcast::all(&widget_home::test2, {"aaa"}, {"bbb"})); */

	/* auto player = this->addChild(make_unique<WMediaPlayer>(MediaType::Audio)); */
	search = bindNew<widget_search>("widget_search");
	search->set_data(system::ls("/dalaran/brinespell/data"));
	search->edit_search->setFocus(true);
	/* search->on_select_event.connect([=](string value){ debug_line(value); }); */

	audio = bindNew<widget_audio>("widget_audio");

	animated_d20 = bindNew<widget_template>("div_animated_d20");
	animated_d20->set_text("<div id=\"div_animated_d20\" class=\"div_animated_d20\"/>");
	doJavaScript("init_animated_d20();");

	dices_area = bindNew<widget_template>("div_dices_area");
	dices_area->set_text("<div id=\"div_dices_area\" class=\"div_dices_area\"/>");
	doJavaScript("init_dices_area('" + this->id() + "');");

	animated_d20->clicked().connect([&](){ doJavaScript("thow_dices_area('2d20');"); });

	// signal binding
	search->on_select_event.connect([&](string filename){ broadcast::all(&widget_home::change_audio_track, "data/" + filename); });
	signal_dice_results.connect(this, &widget_home::dice_results_callback);
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

void widget_home::dice_results_callback(string value)
{
	debug_line(value);
}

