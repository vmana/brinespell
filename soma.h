#ifndef soma_H
#define soma_H

#include <Wt/WApplication.h>
#include <Wt/WEnvironment.h>

#include "config.h"
#include "wt/generic/widget_stack.h"
#include "wt/generic/widget_save_icon.h"
#include "soma_database.h"
#include <Wt/WJavaScript.h>

namespace dbo = Wt::Dbo;
using namespace Wt;

using namespace mana;

class widget_login;
class widget_home;
class user;
class campaign;

class soma : public WApplication
{
	public:
		shared_ptr<soma_database> D;

		// view
		widget_stack *stack;
		widget_login *view_login = NULL;
		widget_home *view_home = NULL;

		// save icon
		widget_save_icon *save_icon;

		dbo::ptr<user> p_user = dbo::ptr<user>();
		dbo::ptr<campaign> p_campaign = dbo::ptr<campaign>();

		soma(const WEnvironment& env);
		static soma* application();
		void init(shared_ptr<soma> sthis);
		void on_login_success(dbo::ptr<user> p_user);
		void on_disconnect();
		void internal_path_handler(const string &path);
		void update_stack_view(WWidget *view);
		int max_screen_width();
		int max_screen_height();
		~soma();
};

#endif // soma_H
