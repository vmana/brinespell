#include "widget_ally.h"
#include "soma.h"
#include "db/player.h"
#include "db/campaign.h"

widget_ally::widget_ally(dbo::ptr<player> p_player) : wcontainer("party/ally")
{
	this->p_player = p_player;
	setStyleClass("widget_ally");

	// set player character image
	avatar_image = bindNew<WText>("avatar_image");
	avatar_image->setStyleClass("avatar_image");
	string filename = strlower(S->p_campaign->name) + "/avatar/" + strlower(p_player->name) + ".png";
	if (file::exists(global::campaign_path + filename))
	{
		avatar_image->decorationStyle().setBackgroundImage("/data/campaign/" + filename);
	}
	else
	{
		// try to load via id
		filename = strlower(S->p_campaign->name) + "/avatar/" + convert::int_string(p_player.id()) + ".png";
		if (file::exists(global::campaign_path + filename))
		{
			avatar_image->decorationStyle().setBackgroundImage("/data/campaign/" + filename);
		}
	}

	// health bar
	health_bar = bindNew<wtemplate>("health_bar", "character/health_bar");
	health_bar->setStyleClass("position_health_bar");
	health_bar->bindString("css_animations", "no-animation");
	current_health_bar = health_bar->bindNew<WText>("current_health_bar");
	current_health_bar->setStyleClass("current_health_bar");
	health_bar_helper = health_bar->bindNew<widget_template>("ring_button_helper");

	// signal binding

	// update values
	dbo_session session;
	int max_hp = p_player->max_hit_points();
	int total_hp = p_player->total_hit_points();

	int percent = 0;
	if (max_hp > 0) percent = (100 * total_hp) / max_hp;
	string helper = convert::int_string(total_hp) + " / " + convert::int_string(max_hp);

	update_health_bar(percent, helper);
}

void widget_ally::update_health_bar(int percent, string helper)
{
	current_health_bar->setHeight(string(convert::int_string(percent) + "%"));

	health_bar_helper->set_text("<div class=\"ring_button_helper_right\">"
	+ helper
	+ "</div>");
}

