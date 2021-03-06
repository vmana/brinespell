#ifndef widget_search_H
#define widget_search_H

#include "wcontainer.h"
#include <Wt/WLineEdit.h>
#include <Wt/WEvent.h>
#include <Wt/WSignal.h>

class widget_search : public wcontainer
{
	protected:
		fuzzy F;
		vector<fuzzy_choice> results;
		vector<widget_template*> wresults;
		int selected = 0;

	public:

		WLineEdit *edit_search;
		widget_template *suggestions;

		Signal<string> on_select_event;
		Signal<> remove_focus_event;

		widget_search();
		void set_data(const vector<string> &data);
		void on_input_changed();
		void on_key_pressed(const WKeyEvent &event);
		void on_enter_pressed();
		void on_select_choice(string text);
		void update_selected();
};

#endif // widget_search_H
