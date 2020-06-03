#ifndef player_H
#define player_H

#include "config.h"
#include "user.h"

class campaign;

class player
{
	public:
		string name;
		bool game_master = false;
		dbo::ptr<user> p_user;
		dbo::ptr<campaign> p_campaign;
		int class_hit_points = 10;
		int damage = 0;
		int tmp_hit_points = 0;

		int strength = 10;
		int dexterity = 10;
		int constitution = 10;
		int intelligence = 10;
		int wisdom = 10;
		int charisma = 10;

		bool inspiration = false;

		template<class Action>
		void persist(Action &a)
		{
			dbo::field(a, name, "name");
			dbo::field(a, game_master, "game_master");
			dbo::field(a, class_hit_points, "class_hit_points");
			dbo::field(a, tmp_hit_points, "tmp_hit_points");
			dbo::field(a, damage, "damage");

			dbo::field(a, strength, "strength");
			dbo::field(a, dexterity, "dexterity");
			dbo::field(a, constitution, "constitution");
			dbo::field(a, intelligence, "intelligence");
			dbo::field(a, wisdom, "wisdom");
			dbo::field(a, charisma, "charisma");

			dbo::field(a, inspiration, "inspiration");
			dbo::belongsTo(a, p_user, dbo::OnDeleteCascade);
			dbo::belongsTo(a, p_campaign, "campaign", dbo::OnDeleteCascade);
		}

		player();
		int bonus_str() const;
		int bonus_dex() const;
		int bonus_con() const;
		int bonus_int() const;
		int bonus_cha() const;
		int bonus_wis() const;
		int max_hit_points() const;
		int total_hit_points() const;
		void set_damage(int dmg);
};

#endif
