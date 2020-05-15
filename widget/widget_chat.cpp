#include "widget_chat.h"
#include "db/user.h"
#include "soma.h"
#include "wt/wt.h"

widget_chat::widget_chat() : wcontainer("chat")
{
	// load css + js lib if not loaded
	S->application()->require("js/scrollbar.min.js");
	S->application()->useStyleSheet("css/scrollbar.css");

	chat_container = bindNew<widget_template>("chat");
	chat_container->setStyleClass("widget_chat hidden");
	chat_container->set_text("${lines}");

	chat_lines = chat_container->bindNew<WText>("lines");
	chat_lines->setStyleClass("widget_chat_lines");
	chat_lines->setText("");

	chat_input = bindNew<WLineEdit>("chat_input");
	chat_input->setStyleClass("widget_chat_input");
	chat_input->setText("");

	/* string lines = ""; */
	/* for (int i = 0; i < 40; i++) */
	/* 	lines += "line " + convert::int_string(i) + "<br />\n"; */
	/* chat_lines->setText(lines); */

	// change scrollbar display, and scrolls to the bottom when the content change
	doJavaScript("init_chat_box('" + chat_container->id() + "');");

	chat_auto_hide.setInterval(chrono::milliseconds(50000));
	chat_auto_hide.timeout().connect(this, &widget_chat::hide_chat_timeout);
	chat_auto_hide.start();

	// signal binding
	// prevent hiding when mouse is over, force show when mouse is over
	chat_container->mouseWentOver().connect([&](){ chat_container->setStyleClass("widget_chat ss-container"); chat_auto_hide.stop(); });
	chat_container->mouseWentOut().connect([&](){ chat_auto_hide.start(); });
	chat_input->enterPressed().connect(this, &widget_chat::on_chat_enter_pressed);
}

void widget_chat::add_message(string message)
{
	// format msg
	string current_time = wt::current_time().toString("HH:mm").toUTF8();
	if (S->p_user) message =
		"<span class=\"widget_chat_timestamp\">"
		+ current_time
		+ "</span>"
		"<span class=\"widget_chat_player_name\">"
		+ S->p_user->login
		+ " </span> : "
		+ message
		+ "<br />\n";

	/* for (int i = 0; i < 30; i++) */
	chat_lines->setText(chat_lines->text() + WString::fromUTF8(message));

	// reset visible & timeout
	chat_container->setStyleClass("widget_chat ss-container");
	chat_auto_hide.stop();
	chat_auto_hide.start();
}

void widget_chat::on_chat_enter_pressed()
{
	// remove xss/script elements
	WText parser;
	parser.setTextFormat(TextFormat::XHTML);
	if (!parser.setText(chat_input->text())) return;

	string message = parser.text().toUTF8();
	if (message == "") return;

	chat_input->setText(""); // clear
	chat_input_event.emit(message);
}

void widget_chat::hide_chat_timeout()
{
	chat_container->setStyleClass("widget_chat ss-container hidden");
}

widget_chat::~widget_chat()
{
	chat_auto_hide.stop();
}
