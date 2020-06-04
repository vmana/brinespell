#include "widget_stats.h"
#include "soma.h"
#include "db/player.h"

widget_stats::widget_stats() : wcontainer("stats")
{
	setStyleClass("widget_stats");

	text_str = bindNew<WText>("text_str");
	text_dex = bindNew<WText>("text_dex");
	text_con = bindNew<WText>("text_con");
	text_int = bindNew<WText>("text_int");
	text_wis = bindNew<WText>("text_wis");
	text_cha = bindNew<WText>("text_cha");

	update_stats();
}
string widget_stats::notation(int value)
{
	string ret = "";
	if (value >= 0) ret = "+";
	ret += convert::int_string(value);
	return ret;
}

void widget_stats::update_stats()
{
	text_str->setText(notation(S->p_player->bonus_str()));
	text_dex->setText(notation(S->p_player->bonus_dex()));
	text_con->setText(notation(S->p_player->bonus_con()));
	text_int->setText(notation(S->p_player->bonus_int()));
	text_wis->setText(notation(S->p_player->bonus_wis()));
	text_cha->setText(notation(S->p_player->bonus_cha()));
}
