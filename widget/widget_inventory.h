#ifndef widget_inventory_H
#define widget_inventory_H

#include "wcontainer.h"
#include <Wt/WTextArea.h>

class widget_inventory : public wcontainer
{
	public:

		WText *close;
		WTextArea *items;

		widget_inventory();
		void update_values();
		void save_values();
		void switch_visibility();
};

#endif // widget_inventory_H

