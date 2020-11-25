#ifndef npc_H
#define npc_H

#include "config.h"

class npc
{
	public:

		string name;
		string token;

		int strength = 10;
		int dexterity = 10;
		int constitution = 10;
		int intelligence = 10;
		int wisdom = 10;
		int charisma = 10;

		int initiative = 0;

		int tmp_strength = 0;
		int tmp_dexterity = 0;
		int tmp_constitution = 0;
		int tmp_intelligence = 0;
		int tmp_wisdom = 0;
		int tmp_charisma = 0;

		npc();
		npc(string filename);

		int bonus_str() const;
		int bonus_dex() const;
		int bonus_con() const;
		int bonus_int() const;
		int bonus_cha() const;
		int bonus_wis() const;
};

#endif
