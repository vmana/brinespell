#ifndef widget_level_H
#define widget_level_H

#include "widget/wcontainer.h"
#include <Wt/WLineEdit.h>
#include <Wt/WTextArea.h>

class widget_level : public wcontainer
{
	public:

		WText *close;
		WLineEdit *level;
		WLineEdit *character_name;
		WLineEdit *character_class;
		WLineEdit *race;

		WText *proficiency;
		WLineEdit *strength;
		WLineEdit *dexterity;
		WLineEdit *constitution;
		WLineEdit *intelligence;
		WLineEdit *wisdom;
		WLineEdit *charisma;
		WLineEdit *armor_class;
		WLineEdit *class_hit_points;
		WLineEdit *speed;
		WTextArea *features;

		Signal<> on_change;

		widget_level();
		void update_values();
		void save_values();
		/* void try_close(); */
		void switch_visibility();
};

#endif // widget_level_H

