#ifndef widget_character_H
#define widget_character_H

#include "widget/wcontainer.h"
#include "widget_details_hp.h"
#include "widget_level.h"
#include "widget_stats.h"
#include "widget_weapon.h"
#include "widget_inventory.h"
#include "widget_notes.h"
#include <Wt/WLineEdit.h>

class widget_character : public wcontainer
{
	public:

		WText *avatar_image;

		// inspiration
		Signal<bool> on_inspiration_event;
		wtemplate *button_inspiration;
		WText *button_inspiration_icon;
		widget_template *button_inspiration_helper;

		// initiative
		Signal<int> on_initiative_event;
		wtemplate *button_initiative;
		WText *button_initiative_icon;
		WLineEdit *initiative;
		widget_template *button_initiative_helper;

		// health bar
		wtemplate *health_bar;
		WText *current_health_bar;
		widget_template *health_bar_helper;

		// level
		wtemplate *button_level;
		WText *button_level_icon;
		WText *button_level_text;
		widget_template *button_level_helper;

		// weapon
		wtemplate *button_weapon;
		WText *button_weapon_icon;
		widget_template *button_weapon_helper;

		// armor class
		wtemplate *button_armor;
		WLineEdit *armor;
		widget_template *button_armor_helper;

		// inventory
		wtemplate *button_inventory;
		WText *button_inventory_icon;

		// spellbook
		wtemplate *button_spellbook;
		WText *button_spellbook_icon;

		// notes
		wtemplate *button_notes;
		WText *button_notes_icon;

		// panels
		widget_details_hp *details_hp;
		widget_level *level;
		widget_stats *stats;
		widget_weapon *weapon;
		widget_inventory *inventory;
		widget_notes *notes;

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
		void update_health_bar(int percent, string helper);

		// level
		void update_level();
		void on_character_level_change();

		// weapon
		void update_weapon();
		void on_weapon_change(int current_weapon, string helper);
		void on_weapon_wheel(const WMouseEvent &e);

		// armor
		void update_armor();
		void on_armor_wheel(const WMouseEvent &e);
		void on_armor_change();

		// inventory
};

#endif // widget_character_H

