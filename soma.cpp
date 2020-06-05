#include "soma.h"
#include "db/user.h"
#include "db/campaign.h"
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
	} catch (dbo::Exception e) { debug_line(e.what()); system::sleep(1000); quit(); }

	// change default encoding for all WString
	WString::setDefaultEncoding(CharEncoding::UTF8);

	// main div - height: 100vh
	main_div = root()->addNew<WContainerWidget>();
	main_div->setStyleClass("main-background");

	// main stack
	stack = main_div->addNew<WStackedWidget>();
	stack->setStyleClass("stack-background");

	// save icon
	/* save_icon = addNew<widget_save_icon>(); */

	// Widgets declaration

	// login widget
	/* view_login = stack->addNew<widget_login>(this); */

	internalPathChanged().connect(this, &soma::internal_path_handler);

	// force init to /
	setInternalPath("/");
	setInternalPath("/login", true);

	try
	{
		dbo_session session;
		p_user = session->find<user>().where("login = 'mana'");
	} catch (dbo::Exception e) { debug_line(e.what()); }
	on_login_success(p_user);
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

	p_campaign = session->find<campaign>().where("name = 'Curse of Strahd'");
	p_player = session->find<player>()
		.where("campaign_id = ? and user_id = ?")
		.bind(p_campaign.id())
		.bind(p_user.id());

	setInternalPath("/home", true);
}

void soma::on_disconnect()
{
	redirect("/");
	quit();
}

void soma::internal_path_handler(const string &path)
{
	cout << "internal path changed : " << path << endl;

	// full access
	if (path == "/login")
	{
		// create if doesn't exist
		if (view_login) stack->removeWidget(view_login);
		view_login = stack->addNew<widget_login>();
		update_stack_view(view_login);
	}
	if (path == "/home")
	{
		// create if doesn't exist
		if (view_home) stack->removeWidget(view_home);
		view_home = stack->addNew<widget_home>();
		update_stack_view(view_home);
	}
}

void soma::update_stack_view(WWidget *view)
{
	stack->setCurrentWidget(view);
	/* view_menu->update_menu(); */
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
	} catch (dbo::Exception e) {}
}

soma::~soma()
{
}
