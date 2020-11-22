#include "wtoken_player.h"
#include "soma.h"

wtoken_player::wtoken_player(dbo::ptr<player> p_player, string id, int top, int left) :
	widget_token(p_player->token_image(), id, top, left)
{
	this->p_player = p_player;
	tooltip->panel_extended_class = "panel_player_extended";
	content = tooltip->panel->bindNew<wcontainer>("content", "token/tooltip_player");

	description = content->bindNew<WText>("description");
	description->setStyleClass("description");
	description->setText("Actions");

	close = content->bindNew<WText>("close");
	close->setStyleClass("close");

	close->mouseWentOver().connect(this, &wtoken_player::on_close_mouseover);
	close->mouseWentOut().connect(this, &wtoken_player::reset_description);
	close->clicked().connect(this, &widget_token::on_close_click);
}

void wtoken_player::reset_description()
{
	description->setText("Actions");
}

void wtoken_player::on_close_mouseover()
{
	description->setText("Close token");
}
