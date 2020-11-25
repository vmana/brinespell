#ifndef wtoken_player_H
#define wtoken_player_H

#include "widget_token.h"
#include <Wt/WEvent.h>
#include <Wt/WSignal.h>
#include <db/player.h>

class wtoken_player : public widget_token
{
	public:

		dbo::ptr<player> p_player;
		wcontainer *content;
		WText *description;
		WText *close;

		wtoken_player(dbo::ptr<player> p_player, string id, int top, int left);
		void reset_description();
		void on_close_mouseover();
};

#endif // wtoken_player_H

