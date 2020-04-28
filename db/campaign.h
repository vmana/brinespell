#ifndef campaign_H
#define campaign_H

#include "config.h"
#include "player.h"

class campaign
{
	public:

		string name;
		dbo::ptr<user> p_master;
		dbo::collection<dbo::ptr<player>> players;

		campaign();

		template<class Action>
		void persist(Action &a)
		{
			dbo::field(a, name, "name");
			dbo::belongsTo(a, p_master, "master", dbo::OnDeleteSetNull);
			dbo::hasMany(a, players, dbo::ManyToOne, "campaign");
		}

		bool load(string name);
};

#endif
