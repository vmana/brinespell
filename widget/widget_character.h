#ifndef widget_portrait_H
#define widget_portrait_H

#include "wcontainer.h"
#include <Wt/WLineEdit.h>

class widget_character : public wcontainer
{
	public:

		WText *avatar_image;

		// inspiration
		Signal<bool> on_inspiration_event;
		wtemplate *button_inspiration;
		WText *button_inspiration_bg;
		widget_template *button_inspiration_helper;

		// initiative
		Signal<int> on_initiative_event;
		wtemplate *button_initiative;
		WText *button_initiative_bg;
		WLineEdit *initiative;
		widget_template *button_initiative_helper;

		// health bar
		wtemplate *health_bar;
		WText *current_health_bar;
		widget_template *health_bar_helper;

		// details hit points
		widget_template *details_hp;
		WText *close_details_hp;
		WText *details_max_hit_points;
		WLineEdit *details_damage;
		WLineEdit *details_tmp_hit_points;
		WText *details_hit_points;

		// stats
		widget_template *stats;

		widget_character();

		// inspiration
		void on_inspiration_click();
		void update_inspiration();

		// initiative
		void on_initiative_change();
		void on_initiative_wheel(const WMouseEvent &e);
		void update_initiative();

		// health
		void on_health_bar_wheel(const WMouseEvent &e);
		void update_hit_point();
		void on_details_hp_change();
		void on_details_hp_wheel(const WMouseEvent &e);
		void on_details_tmp_hp_change();
		void on_details_tmp_hp_wheel(const WMouseEvent &e);
		void switch_details_hp_visibility();
};

#endif // widget_portrait_H

