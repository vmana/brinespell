#ifndef widget_token_H
#define widget_token_H

#include "widget/wcontainer.h"
#include <Wt/WEvent.h>
#include <Wt/WSignal.h>
#include <db/player.h>

class widget_token : public wcontainer
{
	protected:
		bool shared = false; // shared with other players
		bool visible = true;

	public:

		JSignal<int, int> signal_move; // top, left

		Signal<int, int> on_move_event;
		Signal<bool> on_shared_event;
		Signal<> on_close_event;

		widget_token(string filename, bool visible = true);
		widget_token(string filename, string id, bool visible = true);
		widget_token(string filename, string id, int top, int left, bool visible = true);
		void on_close_click();
		void on_shared_click();
		void animate_position(int top, int left);
		void signal_move_callback(int top, int left);
		void close();
		void change_shared(bool shared);
		void change_token_visibility(bool visible); // call when master token shared is changed
};

class widget_drag_token : public wcontainer
{
	public:
		dbo::ptr<player> p_player;

		widget_drag_token();
		widget_drag_token(dbo::ptr<player> p_player);
};

#endif // widget_token_H

