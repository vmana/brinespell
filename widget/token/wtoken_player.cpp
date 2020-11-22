#include "wtoken_player.h"
#include "soma.h"

wtoken_player::wtoken_player(dbo::ptr<player> p_player, string id, int top, int left, bool visible) :
	widget_token(p_player->token_image(), id, top, left, visible)
{
	this->p_player = p_player;
}

