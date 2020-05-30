#ifndef widget_image_H
#define widget_image_H

#include "wcontainer.h"
#include <Wt/WEvent.h>
#include <Wt/WSignal.h>

class widget_image : public wcontainer
{
	protected:
		bool shared = true; // shared with other players
		bool visible = true;

	public:

		WText *button_shared;
		WText *button_close;

		JSignal<int, int> signal_move; // top, left
		JSignal<int, int, int, int> signal_resize; // top, left, width, height
		JSignal<string> signal_view_mode;
		JSignal<int, int, int, int> signal_zoom; // zoom_w, zoom_h, zoom_x,zoom_y

		Signal<int, int> on_move_event;
		Signal<tuple<int, int, int, int>> on_resize_event;
		Signal<string> on_view_mode_event;
		Signal<tuple<int, int, int, int>> on_zoom_event;
		Signal<bool> on_shared_event;
		Signal<> on_close_event;

		widget_image(string filename, bool visible = true);
		widget_image(string filename, string id, bool visible = true);
		void on_close_click();
		void on_shared_click();
		void animate_position(int top, int left);
		void animate_resize(int top, int left, int width, int height);
		void animate_zoom(int zoom_w, int zoom_h, int zoom_x, int zoom_y);
		void signal_move_callback(int top, int left);
		void signal_resize_callback(int top, int left, int width, int height);
		void signal_view_mode_callback(string mode);
		void signal_zoom_callback(int zoom_w, int zoom_h, int zoom_x, int zoom_y);
		void close();
		void change_view_mode(string mode);
		void change_shared(bool shared);
		void change_image_visibility(bool visible); // call when master image shared is changed
};

#endif // widget_image_H

