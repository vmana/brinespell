#include "widget_initiative.h"
#include "soma.h"

/****    wtoken_initiative    ****/

wtoken_initiative::wtoken_initiative()
{
	/* token = make_unique<wcontainer>("token/initiative"); */
	/* token->setStyleClass("wtoken_initiative"); */
}

wtoken_initiative::wtoken_initiative(shared_ptr<npc> p_npc) :
	wtoken_initiative()
{
	this->p_npc = p_npc;
}

wtoken_initiative::wtoken_initiative(dbo::ptr<player> p_player) :
	wtoken_initiative()
{
	this->p_player = p_player;
}

int wtoken_initiative::initiative()
{
	if (p_npc) return p_npc->initiative;
	if (p_player) return p_player->initiative;
	return 0;
}

int wtoken_initiative::dexterity()
{
	if (p_npc) return p_npc->dexterity;
	if (p_player) return p_player->dexterity;
	return 0;
}

/****    widget_initiative    ****/

widget_initiative::widget_initiative()
{
	S = soma::application();

	if (visible) setStyleClass("widget_initiative");
	else setStyleClass("widget_initiative visibility_hidden");

	// load players
	dbo_session session;
	dbo::collection<dbo::ptr<player>> res =
		session->find<player>()
		.where("campaign_id = ? and game_master = 0")
		.bind(S->p_campaign.id());

	auto players = vectorize(res);

	for (auto &p_player : players)
	{
		wtoken_initiative token(p_player);
		token.widget = addNew<WContainerWidget>();
		token.widget->setStyleClass("wtoken_initiative");
		token.widget->decorationStyle().setBackgroundImage(p_player->token_image());
		tokens.push_back(token);
	}

	sort_tokens();
}

void widget_initiative::add_npc(shared_ptr<npc> p_npc)
{
	wtoken_initiative token(p_npc);
	token.widget = addNew<WContainerWidget>();
	token.widget->setStyleClass("wtoken_initiative");
	token.widget->decorationStyle().setBackgroundImage(p_npc->token);
	tokens.push_back(token);

	sort_tokens();
}

void widget_initiative::remove_npc(shared_ptr<npc> p_npc)
{
	for (auto it = tokens.begin(); it < tokens.end(); ++it)
	{
		if ((*it).p_npc == p_npc)
		{
			if ((*it).widget) this->removeWidget((*it).widget);
			tokens.erase(it);
			break;
		}
	}
	sort_tokens();
}

void widget_initiative::reload_player_initiative(int player_id)
{
	dbo_session session;
	for (auto &token : tokens)
	{
		if (token.p_player && token.p_player.id() == player_id)
		{
			token.p_player.reread();
			break;
		}
	}
}

void widget_initiative::sort_tokens()
{
	dbo_session session;
	int size = token_margin + tokens.size() * (token_size + token_margin);
	this->setMargin(- size / 2 , Side::Left);
	this->setWidth(size);

	// sort by initiative
	// when in a tie, use dexterity
	sort(tokens.begin(), tokens.end(),
	[](wtoken_initiative &A, wtoken_initiative &B)
	{
		if (A.initiative() == B.initiative())
			return (A.dexterity() > B.dexterity());
		return (A.initiative() > B.initiative());
	});

	// change positions
	int i = 0;
	for (auto &token : tokens)
	{
		if (!token.widget) continue; // should never happen
		token.widget->setMargin(token_margin + i * (token_size + token_margin), Side::Left);
		token.widget->setToolTip("initiative " + convert::int_string(token.initiative()));
		i++;
	}
}

void widget_initiative::switch_visibility()
{
	visible = ! visible;
	set_visibility(visible);
}

void widget_initiative::set_visibility(bool visible)
{
	this->visible = visible;
	if (visible)
	{
		setStyleClass("widget_initiative animate_show");
	}
	else
	{
		removeStyleClass("visibility_hidden");
		setStyleClass("widget_initiative animate_hide");
	}
}
