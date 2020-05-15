#ifndef widget_chat_H
#define widget_chat_H

#include "wcontainer.h"

class widget_chat : public wcontainer
{
	public:

		WText *chat_lines;
		void add_message(string message);
		widget_chat();
};

#endif // widget_chat_H

