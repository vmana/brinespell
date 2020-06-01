#ifndef widget_portrait_H
#define widget_portrait_H

#include "wcontainer.h"

class widget_portrait : public wcontainer
{
	public:

		WText *avatar_image;
		wtemplate *button_image_spawn;
		WText *button_image_spawn_bg;

		// variable to determine is an image is visible to others when created
		bool spawn_image_visible = false;

		widget_portrait();
		void on_spawn_visible_click();
		void update_spaw_visible(bool visible);
};

#endif // widget_portrait_H

