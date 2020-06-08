#ifndef player_H
#define player_H

#include "config.h"
#include "user.h"

class campaign;
class skill;

class player
{
	public:
		string name;
		dbo::ptr<user> p_user;
		dbo::ptr<campaign> p_campaign;
		bool game_master = false;

		int level = 1;

		int strength = 10;
		int dexterity = 10;
		int constitution = 10;
		int intelligence = 10;
		int wisdom = 10;
		int charisma = 10;

		int initiative = 0;
		bool inspiration = false;

		int class_hit_points = 8;
		int damage = 0;
		int tmp_hit_points = 0;

		int tmp_strength = 0;
		int tmp_dexterity = 0;
		int tmp_constitution = 0;
		int tmp_intelligence = 0;
		int tmp_wisdom = 0;
		int tmp_charisma = 0;

		dbo::collection<dbo::ptr<skill>> skills;

		template<class Action>
		void persist(Action &a)
		{
			dbo::field(a, name, "name");
			dbo::belongsTo(a, p_user, dbo::OnDeleteCascade);
			dbo::belongsTo(a, p_campaign, "campaign", dbo::OnDeleteCascade);
			dbo::field(a, game_master, "game_master");
			dbo::field(a, level, "level");

			dbo::field(a, strength, "strength");
			dbo::field(a, dexterity, "dexterity");
			dbo::field(a, constitution, "constitution");
			dbo::field(a, intelligence, "intelligence");
			dbo::field(a, wisdom, "wisdom");
			dbo::field(a, charisma, "charisma");


			dbo::field(a, initiative, "initiative");
			dbo::field(a, inspiration, "inspiration");

			dbo::field(a, class_hit_points, "class_hit_points");
			dbo::field(a, tmp_hit_points, "tmp_hit_points");
			dbo::field(a, damage, "damage");

			dbo::field(a, tmp_strength, "tmp_strength");
			dbo::field(a, tmp_dexterity, "tmp_dexterity");
			dbo::field(a, tmp_constitution, "tmp_constitution");
			dbo::field(a, tmp_intelligence, "tmp_intelligence");
			dbo::field(a, tmp_wisdom, "tmp_wisdom");
			dbo::field(a, tmp_charisma, "tmp_charisma");

			dbo::hasMany(a, skills, dbo::ManyToOne, "player");
		}

		player();
		int proficiency() const;
		int bonus_str() const;
		int bonus_dex() const;
		int bonus_con() const;
		int bonus_int() const;
		int bonus_cha() const;
		int bonus_wis() const;
		int max_hit_points() const;
		int total_hit_points() const;
		void wound(int dmg); // add new damage
		void set_damage(int dmg); // set damage value
};

#endif
