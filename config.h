#ifndef config_soma_H
#define config_soma_H

#ifndef MANA_WT
#define MANA_WT
#endif

#include <string>
using namespace std;
#include "mana.h"
using namespace mana;

#include "dbo_version.h"

class global
{
	public:
		static string version;
		static bool show_queries;
		static bool production;
		static bool pre_production;
		static bool remote_production;
		static bool try_database_creation;
		static string master_password;
		static string template_path;
		static string data_path;
		static string config_path;
};


#endif // config_soma_H
