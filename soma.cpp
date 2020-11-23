#include "soma.h"
#include "db/user.h"
#include "db/campaign.h"
#include "widget/wcontainer.h"
#include "widget/widget_login.h"
#include "widget/widget_home.h"

soma::soma(const WEnvironment& env) : WApplication(env)
{
	setTitle("~ Brine Spell");

	/****    css & js    ****/
	useStyleSheet("css/style.css");
	useStyleSheet("css/character.css");

	enableUpdates(true); // allow server update

	try
	{
		D = make_shared<soma_database>();
		soma_database::new_session(true);
	} catch (dbo::Exception e) { debug_line(e.what()); system::sleep(1000); return; }

	// change default encoding for all WString
	WString::setDefaultEncoding(CharEncoding::UTF8);

	// main div - height: 100vh
	main_div = root()->addNew<wcontainer>("main");
	main_div->setStyleClass("overlay-background");

	internalPathChanged().connect(this, &soma::internal_path_handler);

	// force init to /
	setInternalPath("/");
	setInternalPath("/login", true);

	// debug purpose, autologin
	if (!global::production)
	{
		try
		{
			dbo_session session;
			p_user = session->find<user>().where("login = 'mana'");
		} catch (dbo::Exception e) { debug_line(e.what()); }
		on_login_success(p_user);
	}
}

/* void soma::callback(string value) { debug_line(value); } */

soma* soma::application()
{
	return (soma*)WApplication::instance();
}

void soma::on_login_success(dbo::ptr<user> p_user)
{
	if (!p_user) return;
	this->p_user = p_user;
	// temporary : force init
	dbo_session session;

	p_player = p_user->players.front();
	p_campaign = p_player->p_campaign;

	// shadow of p_player when joining a campaign
	// used by the game master to impersonate another player with p_player, while retaining its powers
	// for a regular player, p_shadow == p_player and it will never change
	// for the game master, p_player might change, but not it's p_shadow
	p_shadow = p_player;

	setInternalPath("/home", true);
}

void soma::on_disconnect()
{
	redirect("/");
	quit();
}

void soma::internal_path_handler(const string &path)
{
	log("info") << "internal path changed : " << path;
	// reset template
	main_div->load("main");

	// full access
	if (path == "/login")
	{
		view_login = main_div->bindNew<widget_login>("page");
	}
	if (path == "/home")
	{
		view_home = main_div->bindNew<widget_home>("page");
	}
}

int soma::max_screen_width()
{
	return std::max(environment().screenWidth(), environment().screenHeight());
}
int soma::max_screen_height()
{
	return std::min(environment().screenWidth(), environment().screenHeight());
}

void soma::notify(const WEvent& event)
{
	try
	{
		WApplication::notify(event);
	} catch (dbo::Exception e) { debug_line(e.what()); }
}

soma::~soma()
{
}
