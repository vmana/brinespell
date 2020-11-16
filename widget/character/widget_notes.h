#ifndef widget_notes_H
#define widget_notes_H

#include "widget/wcontainer.h"
#include <Wt/WTextArea.h>

class widget_notes : public wcontainer
{
	public:

		WText *close;
		WTextArea *my_notes;

		widget_notes();
		void update_values();
		void save_values();
		void switch_visibility();
};

#endif // widget_notes_H

