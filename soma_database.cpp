#include "soma.h"
#include "soma_database.h"
#include "db/user.h"
#include "db/player.h"
#include "db/skill.h"
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
		p_session.mapClass<skill>("skill");
		p_session.mapClass<base_skill>("base_skill");
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
	create_default_skill();
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
				auto p_player = session->add(move(new_player));

				// add skills
				dbo::collection<dbo::ptr<base_skill>> skills =
					session->find<base_skill>();

				for (auto p_base_skill : skills)
				{
					auto new_skill = make_unique<skill>();
					new_skill->p_player = p_player;
					new_skill->p_base_skill = p_base_skill;
					session->add(move(new_skill));
				}

				// mana game master
				if (character == "Game Master")
				{
					p_player.modify()->game_master = true;
					p_player.modify()->inspiration = true;
					p_player.modify()->level = 6;
					p_player.modify()->class_hit_points = 40;
					p_player.modify()->tmp_hit_points = 10;
					p_player.modify()->damage = 13;
					p_player.modify()->initiative = 18;
					p_player.modify()->strength = 9;
					p_player.modify()->dexterity = 12;
					p_player.modify()->constitution = 12;
					p_player.modify()->intelligence = 14;
					p_player.modify()->wisdom = 16;
					p_player.modify()->charisma = 14;
				}

				session->add(move(new_player));
			}
		}

	} catch (dbo::Exception e) { debug_line(e.what()); }
}

void soma_database::create_default_skill()
{
	try
	{
		dbo_session session;
		// default players
		for (auto &sk : base_skill::listing)
		{
			auto &[attribute,name] = sk;
			// add it as a new user
			dbo::ptr<base_skill> p_skill = session->find<base_skill>()
				.where("name = ? and attribute = ?")
				.bind(name)
				.bind(attribute);
			if (!p_skill)
			{
				auto new_skill = make_unique<base_skill>();
				new_skill->name = name;
				new_skill->attribute = attribute;
				session->add(move(new_skill));
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

