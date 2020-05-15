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

		template<class Action>
		void persist(Action &a)
		{
			dbo::field(a, name, "name");
			dbo::field(a, game_master, "game_master");
			dbo::belongsTo(a, p_user, dbo::OnDeleteCascade);
			dbo::belongsTo(a, p_campaign, "campaign", dbo::OnDeleteCascade);
		}

		player();
};

#endif
