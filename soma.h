#ifndef soma_H
#define soma_H

#include <Wt/WApplication.h>
#include <Wt/WEnvironment.h>
#include <Wt/WJavaScript.h>
#include <Wt/WText.h>
#include <Wt/WStackedWidget.h>

#include "config.h"
#include "soma_database.h"

namespace dbo = Wt::Dbo;
using namespace Wt;

using namespace mana;

class widget_login;
class widget_home;
class user;
class player;
class campaign;


class soma : public WApplication
{
	public:
		shared_ptr<soma_database> D;

		// view
		WContainerWidget *main_div;
		WStackedWidget *stack;
		widget_login *view_login = NULL;
		widget_home *view_home = NULL;

		dbo::ptr<user> p_user = dbo::ptr<user>();
		dbo::ptr<player> p_player = dbo::ptr<player>();
		dbo::ptr<campaign> p_campaign = dbo::ptr<campaign>();
		// shadow of p_player when joining a campaign
		dbo::ptr<player> p_shadow = dbo::ptr<player>();

		soma(const WEnvironment& env);
		static soma* application();
		void init(shared_ptr<soma> sthis);
		void on_login_success(dbo::ptr<user> p_user);
		void on_disconnect();
		void internal_path_handler(const string &path);
		void update_stack_view(WWidget *view);
		int max_screen_width();
		int max_screen_height();
		void notify(const WEvent& event);

		~soma();
};

#endif // soma_H
