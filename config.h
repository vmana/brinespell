#ifndef config_soma_H
#define config_soma_H

#ifndef MANA_WT
#define MANA_WT
#endif

#include <string>
using namespace std;
#include "common.h"
#include "cypher.h"
#include "fuzzy.h"
#include "wt/wt.h"
using namespace mana;

#include "db/dbo_version.h"

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
		static string campaign_path;
		static bool css_animations;

};


#endif // config_soma_H
