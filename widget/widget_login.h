#ifndef widget_login_H
#define widget_login_H

#include "wcontainer.h"

class widget_login : public wcontainer
{
	public:

		WLineEdit *user, *pass;
		widget_login();
};

#endif // widget_login_H

