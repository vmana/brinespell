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
		widget_template *secret_dices_area;
		wtemplate *dice_selector;
		widget_template *animated_d20;
		WText *text_notation;

		// selector buttons
		// decorations => canvas so it's pretty
		widget_template *canvas_decoration_left;
		widget_template *canvas_decoration_right;

		// actual buttons for click purpose
		WText *button_left;
		WText *button_middle;
		WText *button_right;

		JSignal<string, bool> signal_dice_results; // result + is_secret
		JSignal<int, int> signal_selector_click;
		Signal<string, string> throw_dice_event; // notation + rand_init
		Signal<string> dice_results_event;
		Signal<string> dice_secret_results_event;

		widget_dice();
		void on_animated_d20_click();
		void clear_notation();
		void on_throw_click();
		void on_secret_throw_click();

		void throw_dice(string notation); // notation = "2d20 + 4d6"
		void throw_dice(string notation, string rand_init);
		void throw_dice_nocallback(string notation, string rand_init);
		void dice_results_callback(string value, bool is_secret);
		void selector_click_callback(int dice_type, int count); // dice_type 20 for d20 ..., count is either 1 or -1
};

#endif // widget_dice_H

