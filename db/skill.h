#ifndef skill_H
#define skill_H

#include "config.h"
#include "player.h"

class skill;

class base_skill
{
	public:
		dbo::collection<dbo::ptr<skill>> skills;

		static const vector<tuple<string, string>> listing;

		string name;
		string attribute;

		template<class Action>
		void persist(Action &a)
		{
			dbo::field(a, name, "name");
			dbo::field(a, attribute, "attribute");
			dbo::hasMany(a, skills, dbo::ManyToOne, "base_skill");
		}

		base_skill();
};

class skill
{
	public:
		dbo::ptr<player> p_player;
		dbo::ptr<base_skill> p_base_skill;

		int bonus = 0;
		bool proficient = false;

		template<class Action>
		void persist(Action &a)
		{
			dbo::field(a, bonus, "bonus");
			dbo::field(a, proficient, "proficient");
			dbo::belongsTo(a, p_player, dbo::OnDeleteCascade);
			dbo::belongsTo(a, p_base_skill, dbo::OnDeleteSetNull);
		}

		skill();
};

#endif

