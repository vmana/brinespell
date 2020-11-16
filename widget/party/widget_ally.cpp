#include "widget_ally.h"
#include "soma.h"
#include "db/player.h"
#include "db/campaign.h"

widget_ally::widget_ally() : wcontainer("party/ally")
{
	setStyleClass("div_character");

	// css animations
	bindString("css_animations", (global::css_animations) ? "" : "no-animation");

	// set player character image
	avatar_image = bindNew<WText>("avatar_image");
	avatar_image->setStyleClass("avatar_image");
	string filename = strlower(S->p_campaign->name) + "/avatar/" + strlower(S->p_player->name) + ".png";
	if (file::exists(global::campaign_path + filename))
	{
		avatar_image->decorationStyle().setBackgroundImage("/data/campaign/" + filename);
	}
	else
	{
		// try to load via id
		filename = strlower(S->p_campaign->name) + "/avatar/" + convert::int_string(S->p_player.id()) + ".png";
		if (file::exists(global::campaign_path + filename))
		{
			avatar_image->decorationStyle().setBackgroundImage("/data/campaign/" + filename);
		}
	}

	// health bar
	health_bar = bindNew<wtemplate>("health_bar", "character/health_bar");
	health_bar->setStyleClass("position_health_bar");
	health_bar->bindString("css_animations", (global::css_animations) ? "" : "no-animation");
	current_health_bar = health_bar->bindNew<WText>("current_health_bar");
	current_health_bar->setStyleClass("current_health_bar");
	health_bar_helper = health_bar->bindNew<widget_template>("ring_button_helper");
	health_bar_helper->setToolTip("Scroll to change Hit Points");

	// signal binding

	// update values
	/* details_hp->update_hit_point(); */
}

void widget_ally::update_health_bar(int percent, string helper)
{
	current_health_bar->setHeight(string(convert::int_string(percent) + "%"));

	health_bar_helper->set_text("<div class=\"ring_button_helper_right\">"
	+ helper
	+ "</div>");
}

