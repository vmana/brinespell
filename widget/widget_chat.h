#ifndef widget_chat_H
#define widget_chat_H

#include "wcontainer.h"
#include <Wt/WTimer.h>

class widget_chat : public wcontainer
{
	protected:
		WTimer chat_auto_hide;

	public:

		WText *chat_lines;
		void add_message(string message);
		void hide_chat_timeout();
		widget_chat();
		~widget_chat();
};

#endif // widget_chat_H

