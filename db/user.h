#ifndef user_H
#define user_H

#include "config.h"

class player;

class user
{
	public:
		string login;
		string password;
		dbo::collection<dbo::ptr<player>> players;

		template<class Action>
		void persist(Action &a)
		{
			dbo::field(a, login, "login");
			dbo::field(a, password, "password");
			dbo::hasMany(a, players, dbo::ManyToOne, "user");
		}

		user();
		static dbo::ptr<user> check_login(string login, string password);
};

#endif
