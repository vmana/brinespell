#include "widget_party.h"
#include "soma.h"
#include "db/player.h"
#include "db/campaign.h"

widget_party::widget_party() : wcontainer("party/party")
{
	// load css
	S->application()->useStyleSheet("css/party.css");
	setStyleClass("widget_party");
	init_widget();
}

void widget_party::init_widget()
{
	dbo_session session;

	// reset
	allies.clear();

	string template_text;

	dbo::collection<dbo::ptr<player>> res =
		session->find<player>()
		.where("campaign_id = ?")
		.bind(S->p_campaign.id())
		.orderBy("party_order, id");

	auto players = vectorize(res);

	for (auto p_player : players)
	{
		template_text += "${ally_" + convert::int_string(p_player.id()) + "}";
	}
	set_text(template_text);

	// create widget_ally for each player
	for (auto p_player : players)
	{
		auto new_ally = bindNew<widget_ally>("ally_" + convert::int_string(p_player.id()), p_player);
		// compute it's position
		// if allies.size is even, it will be shifted on the right, if odd, no horizontal shift
		// widget_ally height is 128px, width is 108px
		if (allies.size() % 2 == 1)
		{
			new_ally->setMargin(96, Side::Left);
		}

		new_ally->setMargin(allies.size() * 70, Side::Top);

		// signals binding
		if (S->p_shadow->game_master)
		{
			new_ally->avatar_image->clicked().connect(bind(&widget_party::impersonate_player, this, p_player));
		}

		allies.push_back(new_ally);
	}
}

void widget_party::update_hit_point(int player_id, int percent, string helper)
{
	dbo_session session;
	auto ally = search_ally(player_id);
	if (!ally) return;

	ally->update_health_bar(percent, helper);
}

widget_ally* widget_party::search_ally(int player_id)
{
	dbo_session session;
	for (auto &ally : allies)
	{
		if (ally->p_player.id() == player_id) return ally;
	}
	return NULL;
}

void widget_party::impersonate_player(dbo::ptr<player> p_player)
{
	S->p_player = p_player;
	impersonate_event.emit();
}
