#ifndef widget_portrait_H
#define widget_portrait_H

#include "wcontainer.h"

class widget_portrait : public wcontainer
{
	public:


		WText *avatar_image;

		bool inspired = false;
		Signal<bool> on_inspiration_event;
		wtemplate *button_inspiration;
		WText *button_inspiration_bg;
		widget_template *button_inspiration_helper;

		widget_portrait();
		void on_inspiration_click();
		void update_inspiration(bool inspired);
};

#endif // widget_portrait_H

