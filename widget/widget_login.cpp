#include "widget_login.h"
#include "soma.h"
#include "db/user.h"
#include "db/campaign.h"
#include <Wt/WLineEdit.h>
#include <Wt/WTimer.h>

widget_login::widget_login() : wcontainer("login")
{
	setStyleClass("widget_login animate_show");
	edit_login = bindNew<WLineEdit>("edit_login");
	edit_pass = bindNew<WLineEdit>("edit_pass");

	/****    style & values    ****/

	edit_login->setStyleClass("widget_login_input");
	edit_login->setPlaceholderText("Username");

	edit_pass->setStyleClass("widget_login_input");
	edit_pass->setPlaceholderText("Password");
	edit_pass->setEchoMode(EchoMode::Password);

	/****    signal binding    ****/
	edit_login->enterPressed().connect(this, &widget_login::on_enter_pressed);
	edit_pass->enterPressed().connect(this, &widget_login::on_enter_pressed);

	if (!global::production)
	{
		edit_login->setText("mana");
		edit_pass->setText("789brinepass");
	}

	edit_login->setFocus(true);
}

void widget_login::on_enter_pressed()
{
	if (failed_login) return; // don't even bother, wait for reset
	auto p_user = user::check_login(edit_login->text().toUTF8(), edit_pass->text().toUTF8());
	if (p_user)
	{
		S->on_login_success(p_user);
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
	setStyleClass("widget_login widget_login_reset");
	edit_pass->setFocus(true);
	failed_login = false;
}
