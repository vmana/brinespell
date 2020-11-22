#ifndef widget_token_H
#define widget_token_H

#include "widget/wcontainer.h"
#include <Wt/WEvent.h>
#include <Wt/WSignal.h>
#include <db/player.h>

class widget_token_tooltip;

class widget_token : public wcontainer
{
	public:

		widget_token_tooltip *tooltip;

		JSignal<int, int> signal_move; // top, left
		JSignal<> signal_right_click; // used because we disable oncontextmenu

		Signal<int, int> on_move_event;
		Signal<> on_close_event;

		widget_token(string filename);
		widget_token(string filename, string id);
		widget_token(string filename, string id, int top, int left);
		void on_token_click();
		void on_close_click();
		void animate_position(int top, int left);
		void signal_move_callback(int top, int left);
		void close_token();
};

class widget_token_tooltip : public wcontainer
{
	protected:
		bool visible = false;

	public:
		WContainerWidget *bar;
		wcontainer *panel;
		string panel_extended_class = "panel_extended";
		wcontainer *content = NULL;

		widget_token_tooltip();
		virtual void show_panel();
		virtual void hide_panel();
		void switch_visibility();
		void change_visibility(bool visible);
};

class widget_drag_token : public wcontainer
{
	public:
		dbo::ptr<player> p_player;

		widget_drag_token();
		widget_drag_token(dbo::ptr<player> p_player);
};

#endif // widget_token_H

