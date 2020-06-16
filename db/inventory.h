#ifndef inventory_H
#define inventory_H

#include "config.h"

class player;

class inventory
{
	public:
		dbo::ptr<player> p_player;

		int current_weapon = 0;
		int spell_bonus = 0;
		int spell_dc = 8;
		string weapon_1;
		int weapon_1_bonus = 0;
		string weapon_1_damage;
		string weapon_2;
		int weapon_2_bonus = 0;
		string weapon_2_damage;
		string items;

		template<class Action>
		void persist(Action &a)
		{
			dbo::belongsTo(a, p_player, dbo::OnDeleteCascade);
			dbo::field(a, current_weapon, "current_weapon");
			dbo::field(a, spell_bonus, "spell_bonus");
			dbo::field(a, spell_dc, "spell_dc");
			dbo::field(a, weapon_1, "weapon_1");
			dbo::field(a, weapon_1_bonus, "weapon_1_bonus");
			dbo::field(a, weapon_1_damage, "weapon_1_damage");
			dbo::field(a, weapon_2, "weapon_2");
			dbo::field(a, weapon_2_bonus, "weapon_2_bonus");
			dbo::field(a, weapon_2_damage, "weapon_2_damage");
			dbo::field(a, items, "items");
		}

		inventory();
};

#endif

