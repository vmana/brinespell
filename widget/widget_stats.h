#ifndef widget_stats_H
#define widget_stats_H

#include "wcontainer.h"

class widget_stats : public wcontainer
{
	public:

		WText *text_str;
		WText *text_dex;
		WText *text_con;
		WText *text_int;
		WText *text_wis;
		WText *text_cha;

		widget_stats();

		string notation(int value); // return notation value, ie -2, +3 ...
		void update_stats();
};

#endif // widget_stats_H

