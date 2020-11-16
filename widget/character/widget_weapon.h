#ifndef widget_weapon_H
#define widget_weapon_H

#include "widget/wcontainer.h"
#include <Wt/WLineEdit.h>
#include <Wt/WImage.h>

class widget_weapon : public wcontainer
{
	public:

		WText *close;
		WLineEdit *w1_name, *w2_name;
		WLineEdit *spell_bonus, *w1_bonus, *w2_bonus;
		WLineEdit *spell_dc, *w1_damage, *w2_damage;
		WImage *check_spell, *check_weapon_1, *check_weapon_2;

		Signal<int, string> on_change; // current weapon, new weapon helper value

		widget_weapon();
		void update_values();
		void save_values();
		void on_current_weapon_click(int current);
		void switch_visibility();
};

#endif // widget_weapon_H

