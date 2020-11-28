#include "wtoken_npc.h"
#include "soma.h"

wtoken_npc::wtoken_npc(shared_ptr<npc> p_npc, string id, int top, int left) :
	widget_token(p_npc->token, id, top, left)
{
	this->p_npc = p_npc;
	tooltip->panel_extended_class = "panel_npc_extended";
	content = tooltip->panel->bindNew<wcontainer>("content", "token/tooltip_npc");

	description = content->bindNew<WText>("description");
	description->setStyleClass("description");
	description->setText("Actions");

	close = content->bindNew<WText>("close");
	close->setStyleClass("close");

	initiative = content->bindNew<WText>("initiative");
	initiative->setStyleClass("initiative");

	close->mouseWentOver().connect(this, &wtoken_npc::on_close_mouseover);
	close->mouseWentOut().connect(this, &wtoken_npc::reset_description);
	close->clicked().connect(this, &widget_token::on_close_click);

	initiative->mouseWentOver().connect(this, &wtoken_npc::on_initiative_mouseover);
	initiative->mouseWentOut().connect(this, &wtoken_npc::reset_description);
	initiative->clicked().connect(this, &wtoken_npc::on_initiative_click);
}

void wtoken_npc::on_token_click()
{
	if (! S->p_shadow->game_master) return;
	widget_token::on_token_click();
}

void wtoken_npc::reset_description()
{
	description->setText("Actions");
}

void wtoken_npc::on_close_mouseover()
{
	description->setText("Close token");
}

void wtoken_npc::on_initiative_mouseover()
{
	description->setText("Initiative !");
}

void wtoken_npc::on_initiative_click()
{
	p_npc->initiative = 1 + rand() % 20 + p_npc->bonus_dex();
	description->setText("Initiative (" + convert::int_string(p_npc->initiative) + ")");
	on_initiative_event.emit(p_npc->initiative);
}
