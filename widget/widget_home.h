#ifndef widget_home_H
#define widget_home_H

#include "wcontainer.h"
#include "db/campaign.h"

class widget_home : public wcontainer
{
	public:
		shared_ptr<campaign> p_campaign;

		widget_home();
		static void vtest();
		static void test(string a);
		static void test2(string a, string b);
};

#endif // widget_home_H

