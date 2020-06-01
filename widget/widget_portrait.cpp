#include "widget_portrait.h"
#include "soma.h"
#include "db/player.h"
#include "db/campaign.h"

widget_portrait::widget_portrait() : wcontainer("portrait")
{
	// set player character image
	avatar_image = bindNew<WText>("avatar_image");
	avatar_image->setStyleClass("avatar_image");
	string filename = strlower(S->p_campaign->name) + "/avatar/" + strlower(S->p_player->name) + ".png";
	if (file::exists(global::campaign_path + filename))
	{
		avatar_image->decorationStyle().setBackgroundImage("/data/campaign/" + filename);
	}

	// determine spawn_image_visible, for players it's false, for game master it's true
	if (S->p_player->game_master) spawn_image_visible = true;
	else spawn_image_visible = false;

	button_image_spawn = bindNew<wtemplate>("button_image_spawn", "ring_button");
	button_image_spawn->setStyleClass("position_ring_image_spawn");
	button_image_spawn_bg = button_image_spawn->bindNew<WText>("ring_button_bg");

	update_spaw_visible(spawn_image_visible);

	// signal binding
	button_image_spawn->clicked().connect(this, &widget_portrait::on_spawn_visible_click);

}

void widget_portrait::on_spawn_visible_click()
{
	update_spaw_visible(! spawn_image_visible);
}

void widget_portrait::update_spaw_visible(bool visible)
{
	spawn_image_visible = visible;
	if (visible)
	{
		button_image_spawn_bg->setStyleClass("ring_button_bg ring_image_spawn_sun");
	}
	else
	{
		button_image_spawn_bg->setStyleClass("ring_button_bg ring_image_spawn_moon");
	}
}
