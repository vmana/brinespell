#ifndef widget_initiative_H
#define widget_initiative_H

#include "wcontainer.h"
#include "db/npc.h"
#include "db/player.h"

class wtoken_initiative
{
	protected:
		wtoken_initiative();

	public:

		shared_ptr<npc> p_npc;
		dbo::ptr<player> p_player;
		WContainerWidget *widget = NULL;

		wtoken_initiative(shared_ptr<npc> p_npc);
		wtoken_initiative(dbo::ptr<player> p_player);
		int initiative();
		int dexterity();
};

class widget_initiative : public WContainerWidget
{
	public:

		soma *S;
		int token_size = 64;
		int token_margin = 24;
		bool visible = true;

		vector<wtoken_initiative> tokens;

		widget_initiative();
		void add_npc(shared_ptr<npc> p_npc);
		void remove_npc(shared_ptr<npc> p_npc);
		void reload_player_initiative(int player_id);
		void sort_tokens();
		void switch_visibility();
		void set_visibility(bool visible);
};

#endif // widget_initiative_H

