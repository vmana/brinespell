#include "soma.h"
#include "soma_database.h"

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
			mysql = make_unique<dbo::backend::MySQL>("db", "user", "pass", "localhost");
		}
		else
		{
			mysql = make_unique<dbo::backend::MySQL>("db", "user", "pass", "localhost");
			/* mysql = make_unique<dbo::backend::Sqlite3>("/dalaran/sqlite.db"); */
		}

		mysql->setProperty("show-queries", (global::show_queries) ? "true" : "false");

		dbo::Session &p_session = session();

		p_session.setConnection(move(mysql));
		/* p_session.mapClass<brinespell>("brinespell"); */

	}
	catch (dbo::Exception e) { debug_line(e.what()); throw e; }
}

void soma_database::create_database()
{
	try
	{
		dbo_session session;
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

