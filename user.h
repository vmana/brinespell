#ifndef user_H
#define user_H

#include "config.h"

class user
{
	public:
		static yaml user_data;

	user();
	static bool load();
	static bool check_password(string username, string password);
};

#endif
