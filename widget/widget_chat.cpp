#include "widget_chat.h"
#include "db/user.h"
#include "soma.h"

widget_chat::widget_chat() : wcontainer("chat")
{
	// load css + js lib if not loaded
	S->application()->require("js/scrollbar.min.js");
	S->application()->useStyleSheet("css/scrollbar.css");

	setStyleClass("widget_chat");
	chat_lines = bindNew<WText>("lines");
	chat_lines->setStyleClass("widget_chat_lines");
	chat_lines->setText("");
	/* string lines = ""; */
	/* for (int i = 0; i < 40; i++) */
	/* 	lines += "line " + convert::int_string(i) + "<br />\n"; */
	/* chat_lines->setText(lines); */

	// change scrollbar display, and scrolls to the bottom when the content change
	doJavaScript("init_chat_box('" + this->id() + "');");
}

void widget_chat::add_message(string message)
{
	// format msg
	if (S->p_user) message = "<span class=\"widget_chat_player_name\">" + S->p_user->login + " </span> : " + message;
	if (substr(message, -1) != "\n") message += "\n";
	message = str_replace("\n", "<br />\n", message);
	chat_lines->setText(chat_lines->text() + message);
}
