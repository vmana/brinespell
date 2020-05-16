#ifndef widget_dice_H
#define widget_dice_H

#include "wcontainer.h"
#include <Wt/WEvent.h>
#include <Wt/WSignal.h>

class widget_dice : public wcontainer
{
	protected:
		map<int, int> selector_pool =
			{{4,0}, {6,0}, {8,0}, {10,0}, {12,0}, {20,0}, {100,0}};
		string selector_notation;
		void draw_selector();

	public:

		string rand_init;
		widget_template *dices_area;
		wtemplate *dice_selector;
		widget_template *animated_d20;
		WText *text_notation;

		// selector buttons
		widget_template *button_clear;
		WText *button_middle;
		widget_template *button_throw;

		JSignal<string> signal_dice_results;
		JSignal<int, int> signal_selector_click;
		Signal<string, string> throw_dice_event; // notation + rand_init
		Signal<string> dice_results_event;

		widget_dice();
		void on_animated_d20_click();
		void on_clear_click();
		void on_throw_click();

		void throw_dice(string notation); // notation = "2d20 + 4d6"
		void throw_dice(string notation, string rand_init);
		void throw_dice_nocallback(string notation, string rand_init);
		void dice_results_callback(string value);
		void selector_click_callback(int dice_type, int count); // dice_type 20 for d20 ..., count is either 1 or -1
};

#endif // widget_dice_H

