#ifndef widget_image_H
#define widget_image_H

#include "wcontainer.h"
#include <Wt/WEvent.h>
#include <Wt/WSignal.h>

class widget_image : public wcontainer
{
	protected:
		string current_view = "cover";
		bool share = true; // when false, don't emit events

	public:

		WText *button_close;
		WText *button_switch_view;

		JSignal<int, int> signal_move; // top, left
		JSignal<int, int, int, int> signal_resize; // top, left, width, height

		Signal<int, int> on_move_event;
		Signal<tuple<int, int, int, int>> on_resize_event;
		Signal<string> on_switch_view_event;
		Signal<> on_close_event;

		widget_image();
		widget_image(string filename, bool share = true);
		widget_image(string filename, string id, bool share = true);
		void on_close_click();
		void on_switch_view_click();
		void animate_position(int top, int left);
		void animate_resize(int top, int left, int width, int height);
		void signal_move_callback(int top, int left);
		void signal_resize_callback(int top, int left, int width, int height);
		void close();
		void switch_view(string view);
};

#endif // widget_image_H

