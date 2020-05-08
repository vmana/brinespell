#include "widget_home.h"
#include "soma.h"

using namespace std::placeholders;

widget_home::widget_home() : wcontainer("home")
{
	setStyleClass("widget_home");

	/* this->clicked().connect(broadcast::all(&widget_home::test2, {"aaa"}, {"bbb"})); */

	/* auto player = this->addChild(make_unique<WMediaPlayer>(MediaType::Audio)); */
	search = bindNew<widget_search>("widget_search");
	search->set_data(system::ls("/dalaran/brinespell/data"));
	search->edit_search->setFocus(true);
	/* search->on_select_event.connect([=](string value){ debug_line(value); }); */

	audio = bindNew<widget_audio>("widget_audio");

	// signal binding
	search->on_select_event.connect([&](string filename){ broadcast::all(&widget_home::change_audio_track, "data/" + filename); });
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

