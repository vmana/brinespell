#ifndef database_H
#define database_H

#include "config.h"
#include <Wt/Dbo/backend/MySQL.h>
/* #include <Wt/Dbo/backend/Sqlite3.h> */
#include <Wt/Dbo/WtSqlTraits.h>

using namespace Wt;

class soma_database
{
	protected:
		dbo::Session default_dbo_session;
		thread_local static dbo::Session thread_dbo_session;
		thread_local static bool use_thread_session;

	public:

		soma_database();
		static dbo::Session& session();
		static void new_session(bool use_wt_session = false);
		void create_database();
		void create_default();
		void create_default_user();
		void create_default_skill();
		void delete_database();
		static void update_database();
		~soma_database();
};

// session + transaction in a bundle
class dbo_session
{
	public:
		dbo::Transaction transaction;
		dbo_session();
		dbo::Session* operator->();
};


template<class T>
vector<dbo::ptr<T>> vectorize(dbo::collection<dbo::ptr<T>> C)
{
	dbo::Transaction transaction(*C.session());
	vector<dbo::ptr<T>> ret;
	ret.reserve(C.size());
	for (auto &v : C)
	{
		ret.push_back(v);
	}
	return ret;
}

template<class T>
vector<T> vectorize(dbo::collection<T> C)
{
	dbo::Transaction transaction(*C.session());
	vector<T> ret;
	ret.reserve(C.size());
	for (auto &v : C)
	{
		ret.push_back(v);
	}
	return ret;
}


#endif // database_H

