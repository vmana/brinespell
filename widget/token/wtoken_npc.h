#ifndef wtoken_npc_H
#define wtoken_npc_H

#include "widget_token.h"
#include <Wt/WEvent.h>
#include <Wt/WSignal.h>
#include <db/npc.h>

class wtoken_npc : public widget_token
{
	public:

		shared_ptr<npc> p_npc;
		wcontainer *content;
		WText *description;
		WText *initiative;
		WText *close;

		Signal<int> on_initiative_event;

		wtoken_npc(shared_ptr<npc> p_npc, string id, int top, int left);
		void on_token_click();
		void reset_description();
		void on_close_mouseover();
		void on_initiative_mouseover();
		void on_initiative_click();
};

#endif // wtoken_npc_H

