#include "widget_home.h"
#include "soma.h"

using namespace std::placeholders;

widget_home::widget_home() : wcontainer("home")
{
	setStyleClass("widget_home");

	/* this->clicked().connect(broadcast::all(&widget_home::test2, {"aaa"}, {"bbb"})); */

	/* auto player = this->addChild(make_unique<WMediaPlayer>(MediaType::Audio)); */
	search = bindNew<widget_search>("widget_search");
	/* search->set_data(file::read_vector("/home/mana/search.txt")); */
	search->set_data(system::ls("/dalaran/brinespell/data"));
	search->edit_search->setFocus(true);
	search->on_select_event.connect([=](string value){ debug_line(value); });

	auto audio = bindNew<widget_audio>("widget_audio");

	search->on_select_event.connect([=](string value){ audio->load_audio(value); audio->player->play(); });

	/* player->addSource(MediaEncoding::MP3, "template/a.mp3"); */
	/* player->setControlsWidget(0); */
	/* player->setVolume(0.5); */
	/* player->play(); */
}

void widget_home::vtest()
{
	debug_line("vtest");
}

void widget_home::test(string a)
{
	debug_line("test " + a);
}

void widget_home::test2(string a, string b)
{
	string id = soma::instance()->sessionId();
	debug_line("test2 " + a + b);
}
