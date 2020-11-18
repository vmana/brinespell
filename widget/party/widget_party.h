#ifndef widget_party_H
#define widget_party_H

#include "widget/wcontainer.h"
#include "widget_ally.h"

class widget_party : public wcontainer
{
	public:

		vector<widget_ally*> allies;
		Signal<> impersonate_event;

		widget_party();
		void init_widget();
		void update_hit_point(int player_id, int percent, string helper);
		widget_ally* search_ally(int player_id);
		void impersonate_player(dbo::ptr<player> p_player);
};

#endif // widget_party_H

