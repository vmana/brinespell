#ifndef soma_H
#define soma_H

#include <Wt/WApplication.h>
#include <Wt/WEnvironment.h>

#include "config.h"
#include "wt/generic/widget_stack.h"
#include "wt/generic/widget_save_icon.h"
#include "soma_database.h"

namespace dbo = Wt::Dbo;
using namespace Wt;

using namespace mana;

class widget_login;
class widget_home;

class soma : public WApplication
{
	private:
		WGridLayout *main_layout;

	public:
		shared_ptr<soma_database> D;

		bool is_mobile = false;

		// view
		widget_stack *stack;
		widget_login *view_login = NULL;
		widget_home *view_home = NULL;

		// save icon
		widget_save_icon *save_icon;

		soma(const WEnvironment& env);
		static soma* application();
		void init(shared_ptr<soma> sthis);
		void on_disconnect();
		void internal_path_handler(const string &path);
		void update_stack_view(WWidget *view);
		int max_screen_width();
		int max_screen_height();
		~soma();
};

#endif // soma_H
