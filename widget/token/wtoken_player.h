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

		wtoken_player(dbo::ptr<player> p_player, string id, int top, int left, bool visible = true);
};

#endif // wtoken_player_H

