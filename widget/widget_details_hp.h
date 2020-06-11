#ifndef widget_details_hp_H
#define widget_details_hp_H

#include "wcontainer.h"
#include <Wt/WLineEdit.h>

class widget_details_hp : public wcontainer
{
	public:

		WText *close_details_hp;
		WText *details_max_hit_points;
		WLineEdit *details_damage;
		WLineEdit *details_tmp_hit_points;
		WText *details_hit_points;

		Signal<int, string> hit_point_event; // percent, helper

		widget_details_hp();
		void update_hit_point();
		void on_details_hp_change();
		void on_details_hp_wheel(const WMouseEvent &e);
		void on_details_tmp_hp_change();
		void on_details_tmp_hp_wheel(const WMouseEvent &e);
		void switch_details_hp_visibility();
};

#endif // widget_details_hp_H

