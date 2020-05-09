#include "soma.h"
#include "db/user.h"
#include "db/campaign.h"
#include "widget/widget_login.h"
#include "widget/widget_home.h"

soma::soma(const WEnvironment& env) : WApplication(env)
	/* ,sig(this, "unique_name") */
{
	setTitle("Brine Spell");

	/****    css & js    ****/
	useStyleSheet("css/style.css");

	enableUpdates(true); // allow server update

	try
	{
		D = make_shared<soma_database>();
		soma_database::new_session(true);
	} catch (dbo::Exception e) { debug_line(e.what()); system::sleep(1000); quit(); }

	// change default encoding for all WString
	WString::setDefaultEncoding(CharEncoding::UTF8);

	// main div - height: 100vh
	auto main_div = root()->addNew<WContainerWidget>();
	main_div->setStyleClass("main-background");

	// main_layout
	main_layout = main_div->setLayout(make_unique<WGridLayout>());
	main_layout->setContentsMargins(0, 0, 0, 0);
	main_layout->setVerticalSpacing(0);
	main_layout->setHorizontalSpacing(0);

	int widget_count = 0;

	// menu widget
	/* view_menu = main_layout->addWidget( make_unique<widget_menu>(this), widget_count++, 0 ); */
	/* view_menu->hide(); // start hidden */

	// main stack
	stack = main_layout->addWidget( make_unique<widget_stack>(), widget_count++, 0 );
	stack->setStyleClass("stack-background");

	// save icon
	/* save_icon = addNew<widget_save_icon>(); */

	// Widgets declaration

	// login widget
	/* view_login = stack->addNew<widget_login>(this); */

	internalPathChanged().connect(this, &soma::internal_path_handler);

	// force init to /
	setInternalPath("/");

	main_layout->setRowStretch(widget_count - 1, 1); // stack always gets full space

	/* this->doJavaScript("var xxx = 42;"); */
	/* sig.connect(this, &soma::callback); */
	/* debug_line(sig.createCall({"xxx"})); */
	/* this->doJavaScript(sig.createCall({"xxx"})); */
	/* this->doJavaScript("Wt.emit('app','unique_name','ok ok');"); */

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
	try
	{
		dbo_session session;
		p_campaign = session->find<campaign>().where("name = 'Curse of Strahd'");

		// test bad campaign for broadcast
		if (p_user->login == "bad") p_campaign = session->find<campaign>().where("name = 'bad'");

	} catch (dbo::Exception e) { debug_line(e.what()); }

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

soma::~soma()
{
}
