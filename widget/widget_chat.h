#ifndef widget_chat_H
#define widget_chat_H

#include "wcontainer.h"
#include <Wt/WTimer.h>
#include <Wt/WLineEdit.h>

class widget_chat : public wcontainer
{
	protected:
		WTimer chat_auto_hide;

	public:

		widget_template *chat_container;
		WText *chat_lines;
		WLineEdit *chat_input;

		Signal<string> chat_input_event;

		void add_message(string message);
		void on_chat_enter_pressed();
		void hide_chat_timeout();
		widget_chat();
		~widget_chat();
};

#endif // widget_chat_H

