#include "widget_home.h"
#include "soma.h"
#include <Wt/WMediaPlayer.h>
#include <Wt/WText.h>
#include <Wt/WFileUpload.h>

using namespace std::placeholders;

widget_home::widget_home() : wcontainer("home")
{
	setStyleClass("widget_home");

	/* this->clicked().connect(bind(&core::send_all<widget_home, string>, C, this, &widget_home::test, "AAA")); */

	/* this->clicked().connect(bind(&core::send_all<widget_home, string, string>, C, this, &widget_home::test2, "AAA", "BBB")); */

	/* this->clicked().connect(f); */
	this->clicked().connect(broadcast::all(&widget_home::test2, {"aaa"}, {"bbb"}));

	/* auto player = this->addChild(make_unique<WMediaPlayer>(MediaType::Audio)); */
	/* auto player = this->bindNew<WMediaPlayer>("player", MediaType::Audio); */
	search = bindNew<widget_search>("widget_search");

	/* player->addSource(MediaEncoding::MP3, "template/a.mp3"); */
	/* player->setControlsWidget(0); */
	/* player->setVolume(0.5); */
	/* player->play(); */

	/* auto button_minus = bindNew<WText>("minus"); */
	/* button_minus->setText("-"); */
	/* button_minus->clicked().connect([=](){ player->setVolume(player->volume() - 0.1); }); */

	/* auto button_plus = bindNew<WText>("plus"); */
	/* button_plus->setText("+"); */
	/* button_plus->clicked().connect([=](){ player->setVolume(player->volume() + 0.1); }); */

	/* sound->setLoops(1); */
	/* sound->play(); */
	/* map<string, int> test = {{"aaa", 1}, {"bbb", 2}}; */
	/* auto it = test.find("aaa"); */
	/* if (it != test.end()) debug_line(it->second); */
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
