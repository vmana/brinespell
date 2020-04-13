#ifndef widget_login_H
#define widget_login_H

#include "wcontainer.h"

class widget_login : public wcontainer
{
	protected:
		bool failed_login = false;

	public:

		WLineEdit *edit_login, *edit_pass;
		widget_login();
		void on_enter_pressed();
		void reset_login();
};

#endif // widget_login_H

