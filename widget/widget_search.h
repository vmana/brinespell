#ifndef widget_search_H
#define widget_search_H

#include "wcontainer.h"
#include <Wt/WLineEdit.h>
#include <Wt/WEvent.h>

class widget_search : public wcontainer
{
	protected:
		fuzzy F;

	public:

		WLineEdit *edit_search;
		widget_template *suggestions;

		widget_search();
		void set_data(const vector<string> &data);
		void on_key_pressed();
		void on_select_choice(string value);
};

#endif // widget_search_H
