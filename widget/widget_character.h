#ifndef widget_portrait_H
#define widget_portrait_H

#include "wcontainer.h"

class widget_character : public wcontainer
{
	public:


		WText *avatar_image;

		bool inspired = false;
		Signal<bool> on_inspiration_event;
		wtemplate *button_inspiration;
		WText *button_inspiration_bg;
		widget_template *button_inspiration_helper;

		wtemplate *health_bar;
		WText *current_health_bar;
		widget_template *health_bar_helper;

		widget_character();
		void on_inspiration_click();
		void update_inspiration(bool inspired);
		void on_health_bar_wheel(const WMouseEvent &e);
		void update_hit_point();
};

#endif // widget_portrait_H

