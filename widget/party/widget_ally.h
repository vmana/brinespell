#ifndef widget_ally_H
#define widget_ally_H

#include "widget/wcontainer.h"
#include <Wt/WLineEdit.h>

class widget_ally : public wcontainer
{
	public:

		WText *avatar_image;

		// health bar
		wtemplate *health_bar;
		WText *current_health_bar;
		widget_template *health_bar_helper;

		widget_ally();

		// health
		void update_health_bar(int percent, string helper);
};

#endif // widget_ally_H

