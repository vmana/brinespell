#include "widget_character.h"
#include "soma.h"
#include "db/player.h"
#include "db/campaign.h"

widget_character::widget_character() : wcontainer("character")
{
	// set player character image
	avatar_image = bindNew<WText>("avatar_image");
	avatar_image->setStyleClass("avatar_image");
	string filename = strlower(S->p_campaign->name) + "/avatar/" + strlower(S->p_player->name) + ".png";
	if (file::exists(global::campaign_path + filename))
	{
		avatar_image->decorationStyle().setBackgroundImage("/data/campaign/" + filename);
	}

	button_inspiration = bindNew<wtemplate>("button_inspiration", "ring_button");
	button_inspiration->setStyleClass("position_ring_inspiration");
	button_inspiration_bg = button_inspiration->bindNew<WText>("ring_button_bg");
	button_inspiration_helper = button_inspiration->bindNew<widget_template>("ring_button_helper");
	button_inspiration_helper->set_text("<div class=\"ring_button_helper_left\">Inspiration</div>");

	health_bar = bindNew<wtemplate>("health_bar", "health_bar");
	health_bar->setStyleClass("position_health_bar");
	current_health_bar = health_bar->bindNew<WText>("current_health_bar");
	current_health_bar->setStyleClass("current_health_bar");

	// update values from database
	inspired = S->p_player->inspiration;

	update_inspiration(inspired);
	update_hit_point();

	// signal binding
	button_inspiration->clicked().connect(this, &widget_character::on_inspiration_click);

}

void widget_character::on_inspiration_click()
{
	update_inspiration(! inspired);
	on_inspiration_event.emit(inspired);
	// update db
	dbo_session session;
	S->p_player.modify()->inspiration = inspired;
}

void widget_character::update_inspiration(bool inspired)
{
	this->inspired = inspired;
	if (inspired)
	{
		button_inspiration_bg->setStyleClass("ring_button_bg ring_inspiration_sun");
	}
	else
	{
		button_inspiration_bg->setStyleClass("ring_button_bg ring_inspiration_moon");
	}
}

void widget_character::update_hit_point()
{
	dbo_session session;

	int percent;

	if (S->p_player->max_hit_points > 0)
		percent = (100 * S->p_player->hit_points) / S->p_player->max_hit_points;
	else
		percent = 0;
	current_health_bar->setHeight(string(convert::int_string(percent) + "%"));
}
