#include "widget_weapon.h"
#include "soma.h"

widget_weapon::widget_weapon() : wcontainer("character/weapon")
{
	setStyleClass("visibility_hidden");

	close = bindNew<WText>("close");
	close->setStyleClass("close");

	check_spell = bindNew<WImage>("check_spell", "/img/check-off.svg");
	check_weapon_1 = bindNew<WImage>("check_weapon_1", "/img/check-off.svg");
	check_weapon_2 = bindNew<WImage>("check_weapon_2", "/img/check-off.svg");
	check_spell->setStyleClass("check");
	check_weapon_1->setStyleClass("check");
	check_weapon_2->setStyleClass("check");

	w1_name = bindNew<WLineEdit>("w1_name");
	w2_name = bindNew<WLineEdit>("w2_name");
	spell_bonus = bindNew<WLineEdit>("spell_bonus");
	w1_bonus = bindNew<WLineEdit>("w1_bonus");
	w2_bonus = bindNew<WLineEdit>("w2_bonus");
	spell_dc = bindNew<WLineEdit>("spell_dc");
	w1_damage = bindNew<WLineEdit>("w1_damage");
	w2_damage = bindNew<WLineEdit>("w2_damage");

	// signal binding
	close->clicked().connect(this, &widget_weapon::switch_visibility);
}

void widget_weapon::switch_visibility()
{
	if (hasStyleClass("animate_show"))
	{
		removeStyleClass("visibility_hidden");
		setStyleClass("animate_hide");
	}
	else
	{
		setStyleClass("animate_show");
	}
}
