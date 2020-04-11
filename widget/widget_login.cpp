#include "widget_login.h"
#include "soma.h"
#include "user.h"
#include <Wt/WLineEdit.h>
#include <Wt/WTimer.h>

widget_login::widget_login() : wcontainer("login")
{
	setStyleClass("widget_login animate_show");
	edit_user = bindNew<WLineEdit>("edit_user");
	edit_pass = bindNew<WLineEdit>("edit_pass");

	/****    style & values    ****/

	edit_user->setStyleClass("widget_login_input");
	edit_user->setPlaceholderText("Username");

	edit_pass->setStyleClass("widget_login_input");
	edit_pass->setPlaceholderText("Password");
	edit_pass->setEchoMode(EchoMode::Password);

	/****    signal binding    ****/
	edit_user->enterPressed().connect(this, &widget_login::on_enter_pressed);
	edit_pass->enterPressed().connect(this, &widget_login::on_enter_pressed);

	if (!global::production)
	{
		edit_user->setText("mana");
		edit_pass->setText("789manapass");
	}

	edit_user->setFocus(true);
}

void widget_login::on_enter_pressed()
{
	if (failed_login) return; // don't even bother, wait for reset

	if (user::check_password(edit_user->text().toUTF8(), edit_pass->text().toUTF8()))
	{
		// login success
		S->setInternalPath("/home", true);
	}
	else
	{
		// login failed
		edit_pass->setText("");
		failed_login = true;
		setStyleClass("widget_login widget_login_failed");
		WTimer::singleShot(chrono::milliseconds(1000), this, &widget_login::reset_login);
	}
}

void widget_login::reset_login()
{
	edit_pass->setFocus(true);
	setStyleClass("widget_login animate_show");
	failed_login = false;
}
