#include "soma.h"
#include "soma_database.h"
#include "db/user.h"
#include "db/player.h"
#include "db/campaign.h"

thread_local dbo::Session soma_database::thread_dbo_session;
thread_local bool soma_database::use_thread_session = false;

soma_database::soma_database()
{
}

dbo::Session& soma_database::session()
{
	if (soma::application() && !use_thread_session)
	{
		return soma::application()->D->default_dbo_session;
	}
	else
	{
		// not in a wt session
		return soma_database::thread_dbo_session;
	}
}

void soma_database::new_session(bool use_wt_session)
{
	// force current thread to use a session different from default
	soma_database::use_thread_session = !use_wt_session;

	try
	{
		unique_ptr<dbo::SqlConnection> mysql;
		if (global::production)
		{
			mysql = make_unique<dbo::backend::MySQL>("brinespell", "brinespell", "789brinespell", "localhost");
		}
		else
		{
			mysql = make_unique<dbo::backend::MySQL>("brinespell", "brinespell", "789brinespell", "localhost");
			/* mysql = make_unique<dbo::backend::Sqlite3>("/dalaran/sqlite.db"); */
		}

		mysql->setProperty("show-queries", (global::show_queries) ? "true" : "false");

		dbo::Session &p_session = session();

		p_session.setConnection(move(mysql));
		p_session.mapClass<user>("user");
		p_session.mapClass<player>("player");
		p_session.mapClass<campaign>("campaign");

	}
	catch (dbo::Exception e) { debug_line(e.what()); throw e; }
}

void soma_database::create_database()
{
	try
	{
		dbo_session session;
		/* cout << session->tableCreationSql() << endl; */
		session->createTables();
	}
	catch (dbo::Exception e)
	{
		// don't print messages : table already exists
		if (strpos(e.what(), "already exists") == string::npos)
			debug_line(e.what());
	}
}

void soma_database::create_default()
{
	create_default_user();
}

void soma_database::create_default_user()
{
	try
	{
		dbo_session session;
		dbo::ptr<user> p_user;

		// create default campaign
		string campaign_name = "Curse of Strahd";
		dbo::ptr<campaign> p_campaign = session->find<campaign>().where("name = ?").bind(campaign_name);
		if (!p_campaign)
		{
			auto new_campaign = make_unique<campaign>();
			new_campaign->name = campaign_name;
			p_campaign = session->add(move(new_campaign));
		}

		// default players
		vector<tuple<string, string>> player_names = {{"mana","Game Master"}, {"bob","Bob the Warrior"}, {"john","John the Wizard"}};
		for (auto &pn : player_names)
		{
			auto &[login,character] = pn;
			// add it as a new user
			p_user = session->find<user>().where("login = ?").bind(login);
			if (!p_user)
			{
				auto new_user = make_unique<user>();
				new_user->login = login;
				new_user->password = cypher::sha_string("789brinepass");
				p_user = session->add(move(new_user));

				// add user as a player
				auto new_player = make_unique<player>();
				new_player->name = character;
				new_player->p_user = p_user;
				new_player->p_campaign = p_campaign;

				// mana game master
				if (character == "Game Master")
				{
					new_player->game_master = true;
					new_player->inspiration = true;
					new_player->class_hit_points = 40;
					new_player->tmp_hit_points = 10;
					new_player->damage = 13;
					new_player->initiative = 18;
				}

				session->add(move(new_player));
			}
		}

	} catch (dbo::Exception e) { debug_line(e.what()); }
}

void soma_database::delete_database()
{
	try
	{
		dbo_session session;
		session->dropTables();
	} catch (dbo::Exception e) { debug_line(e.what()); }
}

void soma_database::update_database()
{
}

soma_database::~soma_database()
{
}

/****    dbo_session    ****/

dbo_session::dbo_session() :
	transaction(soma_database::session())
{
}

dbo::Session* dbo_session::operator->()
{
	return &soma_database::session();
}

