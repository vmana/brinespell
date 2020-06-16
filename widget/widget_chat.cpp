#include "widget_chat.h"
#include "db/player.h"
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
	chat_input->setToolTip("Global HotKey : Shift + Enter");

	// change scrollbar display, and scrolls to the bottom when the content change
	doJavaScript("init_chat_box('" + chat_container->id() + "');");

	chat_auto_hide.setInterval(chrono::milliseconds(18000));
	chat_auto_hide.timeout().connect(this, &widget_chat::hide_chat_timeout);
	chat_auto_hide.start();

	// signal binding
	// prevent hiding when mouse is over, force show when mouse is over
	chat_container->mouseWentOver().connect([&](){ chat_container->setStyleClass("widget_chat ss-container"); });
	chat_container->mouseWentOut().connect([&](){ if (!chat_auto_hide.isActive()) chat_container->setStyleClass("widget_chat ss-container hidden"); });
	chat_container->clicked().connect(this, &widget_chat::on_chat_click);
	chat_input->keyWentDown().connect(this, &widget_chat::on_key_pressed);
	chat_input->focussed().connect(this, &widget_chat::reset_hide_timer);
	chat_input->clicked().connect(this, &widget_chat::reset_hide_timer);
}

string widget_chat::prepare_message(string message)
{
	soma *p_soma = soma::application();
	if (!p_soma) return message;
	if (!p_soma->p_player) return message;

	// format msg
	string current_time = wt::current_time().toString("HH:mm").toUTF8();
	message =
		"<span class=\"widget_chat_timestamp\">"
		+ current_time
		+ "</span>"
		"<span class=\"widget_chat_player_name\">"
		+ p_soma->p_player->name
		+ " </span> : "
		+ message
		+ "<br />\n";
	return message;
}

void widget_chat::add_message(string message)
{
	chat_lines->setText(chat_lines->text() + WString::fromUTF8(message));

	// reset visible & timeout
	reset_hide_timer();
}

void widget_chat::reset_hide_timer()
{
	chat_container->setStyleClass("widget_chat ss-container");
	chat_auto_hide.stop();
	chat_auto_hide.start();
}

void widget_chat::on_key_pressed(const WKeyEvent &event)
{
	if (event.key() == Key::Enter)
	{
		reset_hide_timer();
		on_chat_enter_pressed();
	}
	else if (event.key() == Key::Escape)
	{
		// remove focus if escape is pressed on an empty input
		if (chat_input->text() == "") remove_focus_event.emit();
		chat_input->setText(""); // clear
	}
	else
	{
		// every other keys
		reset_hide_timer();
	}
}

void widget_chat::on_chat_enter_pressed()
{
	// remove xss/script elements
	WText parser;
	parser.setTextFormat(TextFormat::XHTML);
	if (!parser.setText(chat_input->text())) return;

	string message = parser.text().toUTF8();
	if (message == "")
	{
		remove_focus_event.emit(); // remove focus when enter pressed with empty text
		return;
	}

	message = prepare_message(message);

	chat_input->setText(""); // clear
	chat_input_event.emit(message);
}

void widget_chat::hide_chat_timeout()
{
	chat_container->setStyleClass("widget_chat ss-container hidden");
	chat_auto_hide.stop();
}

void widget_chat::on_chat_click()
{
	hide_chat_timeout();
}

widget_chat::~widget_chat()
{
	chat_auto_hide.stop();
}
