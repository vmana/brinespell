#include "user.h"

yaml user::user_data = yaml(global::config_path + "user.yml");

user::user()
{
}

bool user::load()
{
	bool ret = true;
	ret = user_data.load(global::config_path + "user.yml");
	return ret;
}

bool user::check_password(string username, string password)
{
	bool ret = false;
	if (strpos(username, "/") != string::npos) return ret; // disable username with such name, possible security threat
	string res = user_data.value("user/" + username + "/pass");
	if (res != "" && (res == password || res == cypher::sha_string(password))) ret = true;
	return ret;
}
