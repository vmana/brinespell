#include "widget_login.h"
#include "soma.h"
#include <Wt/WLineEdit.h>

widget_login::widget_login() : wcontainer("login")
{
	setStyleClass("widget_login");
	user = bindNew<WLineEdit>("user");
	pass = bindNew<WLineEdit>("pass");

	/****    style & values    ****/

	user->setStyleClass("widget_login_input");
	user->setPlaceholderText("Username");

	pass->setStyleClass("widget_login_input");
	pass->setPlaceholderText("Password");
	pass->setEchoMode(EchoMode::Password);
}
