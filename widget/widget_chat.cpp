#include "widget_chat.h"
#include "db/user.h"
#include "soma.h"

widget_chat::widget_chat() : wcontainer("chat")
{
	// load css + js lib if not loaded
	S->application()->require("js/scrollbar.min.js");
	S->application()->useStyleSheet("css/scrollbar.css");

	setStyleClass("widget_chat hidden");
	chat_lines = bindNew<WText>("lines");
	chat_lines->setStyleClass("widget_chat_lines");
	chat_lines->setText("");
	/* string lines = ""; */
	/* for (int i = 0; i < 40; i++) */
	/* 	lines += "line " + convert::int_string(i) + "<br />\n"; */
	/* chat_lines->setText(lines); */

	// change scrollbar display, and scrolls to the bottom when the content change
	doJavaScript("init_chat_box('" + this->id() + "');");

	chat_auto_hide.setInterval(chrono::milliseconds(50000));
	chat_auto_hide.timeout().connect(this, &widget_chat::hide_chat_timeout);
	chat_auto_hide.start();

	// signal binding
	// prevent hiding when mouse is over, force show when mouse is over
	this->mouseWentOver().connect([&](){ this->setStyleClass("widget_chat ss-container"); chat_auto_hide.stop(); });
	this->mouseWentOut().connect([&](){ chat_auto_hide.start(); });
}

void widget_chat::add_message(string message)
{
	// format msg
	if (S->p_user) message = "<span class=\"widget_chat_player_name\">" + S->p_user->login + " </span> : " + message;
	if (substr(message, -1) != "\n") message += "\n";
	message = str_replace("\n", "<br />\n", message);

	/* for (int i = 0; i < 30; i++) */
	chat_lines->setText(chat_lines->text() + message);

	// reset visible & timeout
	this->setStyleClass("widget_chat ss-container");
	chat_auto_hide.stop();
	chat_auto_hide.start();
}

void widget_chat::hide_chat_timeout()
{
	this->setStyleClass("widget_chat ss-container hidden");
}

widget_chat::~widget_chat()
{
	chat_auto_hide.stop();
}
