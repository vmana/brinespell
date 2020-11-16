#include "widget_weapon.h"
#include "soma.h"
#include "db/player.h"

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

	spell_bonus->setToolTip("proficiency bonus + main casting modifier");
	spell_dc->setToolTip("8 + proficiency bonus + main casting modifier");
	w1_name->setPlaceholderText("Weapon name");
	w2_name->setPlaceholderText("Weapon name");
	w1_bonus->setToolTip("[proficiency bonus] + strength or dexterity modifier");
	w2_bonus->setToolTip("[proficiency bonus] + strength or dexterity modifier");
	w1_damage->setPlaceholderText("1d8+3");
	w2_damage->setPlaceholderText("1d8+3");

	update_values();

	// signal binding
	close->clicked().connect(this, &widget_weapon::switch_visibility);
	spell_bonus->changed().connect(this, &widget_weapon::save_values);
	spell_dc->changed().connect(this, &widget_weapon::save_values);
	w1_name->changed().connect(this, &widget_weapon::save_values);
	w1_bonus->changed().connect(this, &widget_weapon::save_values);
	w1_damage->changed().connect(this, &widget_weapon::save_values);
	w2_name->changed().connect(this, &widget_weapon::save_values);
	w2_bonus->changed().connect(this, &widget_weapon::save_values);
	w2_damage->changed().connect(this, &widget_weapon::save_values);
	check_spell->clicked().connect([&](){ on_current_weapon_click(0); });
	check_weapon_1->clicked().connect([&](){ on_current_weapon_click(1); });
	check_weapon_2->clicked().connect([&](){ on_current_weapon_click(2); });
}

void widget_weapon::update_values()
{
	dbo_session session;
	auto &p_inventory = S->p_player->p_inventory;
	spell_bonus->setText(convert::int_string(p_inventory->spell_bonus));
	spell_dc->setText(convert::int_string(p_inventory->spell_dc));
	w1_name->setText(p_inventory->weapon_1);
	w1_bonus->setText(convert::int_string(p_inventory->weapon_1_bonus));
	w1_damage->setText(p_inventory->weapon_1_damage);
	w2_name->setText(p_inventory->weapon_2);
	w2_bonus->setText(convert::int_string(p_inventory->weapon_2_bonus));
	w2_damage->setText(p_inventory->weapon_2_damage);

	string helper = "";
	switch (p_inventory->current_weapon)
	{
		case 0:
			check_spell->setImageLink("/img/check-on.svg");
			check_weapon_1->setImageLink("/img/check-off.svg");
			check_weapon_2->setImageLink("/img/check-off.svg");
			helper = "Hit +" + spell_bonus->text().toUTF8() + " / DC " + spell_dc->text().toUTF8();
			break;
		case 1:
			check_spell->setImageLink("/img/check-off.svg");
			check_weapon_1->setImageLink("/img/check-on.svg");
			check_weapon_2->setImageLink("/img/check-off.svg");
			helper = "Hit +" + w1_bonus->text().toUTF8() + " / " + w1_damage->text().toUTF8();
			break;
		case 2:
			check_spell->setImageLink("/img/check-off.svg");
			check_weapon_1->setImageLink("/img/check-off.svg");
			check_weapon_2->setImageLink("/img/check-on.svg");
			helper = "Hit +" + w2_bonus->text().toUTF8() + " / " + w2_damage->text().toUTF8();
			break;
	}

	// remove +-1 when negative modifier
	helper = str_replace("+-", "-", helper);
	// emit signal for parent
	on_change.emit(p_inventory->current_weapon, helper);
}

void widget_weapon::save_values()
{
	dbo_session session;
	auto &p_inventory = S->p_player->p_inventory;

	p_inventory.modify()->spell_bonus = convert::string_int(spell_bonus->text().toUTF8());
	p_inventory.modify()->spell_dc = convert::string_int(spell_dc->text().toUTF8());
	p_inventory.modify()->weapon_1 = w1_name->text().toUTF8();
	p_inventory.modify()->weapon_1_bonus = convert::string_int(w1_bonus->text().toUTF8());
	p_inventory.modify()->weapon_1_damage = w1_damage->text().toUTF8();
	p_inventory.modify()->weapon_2 = w2_name->text().toUTF8();
	p_inventory.modify()->weapon_2_bonus = convert::string_int(w2_bonus->text().toUTF8());
	p_inventory.modify()->weapon_2_damage = w2_damage->text().toUTF8();

	update_values();
}

void widget_weapon::on_current_weapon_click(int current)
{
	if (current < 0 || current > 2) return; // should never happen
	dbo_session session;
	auto &p_inventory = S->p_player->p_inventory;
	p_inventory.modify()->current_weapon = current;
	update_values();
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
