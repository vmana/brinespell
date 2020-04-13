#ifndef player_H
#define player_H

#include "config.h"
#include "user.h"

class player
{
	public:
		string name;
		dbo::ptr<user> p_user;

		template<class Action>
		void persist(Action &a)
		{
			dbo::field(a, name, "login");
			dbo::belongsTo(a, p_user, dbo::OnDeleteCascade);
		}

		player();
};

#endif
