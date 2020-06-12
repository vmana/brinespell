#ifndef widget_details_hp_H
#define widget_details_hp_H

#include "wcontainer.h"
#include <Wt/WLineEdit.h>

class widget_details_hp : public wcontainer
{
	public:

		WText *close;
		WText *max_hit_points;
		WLineEdit *damage;
		WLineEdit *tmp_hit_points;
		WText *hit_points;

		Signal<int, string> hit_point_event; // percent, helper

		widget_details_hp();
		void update_hit_point();
		void on_hp_change();
		void on_hp_wheel(const WMouseEvent &e);
		void on_tmp_hp_change();
		void on_tmp_hp_wheel(const WMouseEvent &e);
		void switch_visibility();
};

#endif // widget_details_hp_H

