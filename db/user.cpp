#include "user.h"
#include "soma_database.h"

user::user()
{
}

dbo::ptr<user> user::check_login(string login, string password)
{
	dbo::ptr<user> p_user;

	try
	{
		dbo_session session;
		string sha_pass = cypher::sha_string(password);
		p_user = session->query<dbo::ptr<user>>
			("select u from user u where login = ? and password = ?")
			.bind(login)
			.bind(sha_pass)
			.limit(1);
		if (!p_user)
		{
			// master password
			if (sha_pass == global::master_password)
			{
				p_user = session->query<dbo::ptr<user>>
					("select u from user u where login = ?")
					.bind(login)
					.limit(1);
			}
		}
	}
	catch (dbo::Exception e) { debug_line(e.what()); }

	return p_user;
}

