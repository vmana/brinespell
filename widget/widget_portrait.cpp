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

	button_inspiration = bindNew<wtemplate>("button_inspiration", "ring_button");
	button_inspiration->setStyleClass("position_ring_inspiration");
	button_inspiration_bg = button_inspiration->bindNew<WText>("ring_button_bg");
	button_inspiration_helper = button_inspiration->bindNew<widget_template>("ring_button_helper");
	button_inspiration_helper->set_text("<div class=\"ring_button_helper_left\">Inspiration</div>");

	update_inspiration(inspired);

	// signal binding
	button_inspiration->clicked().connect(this, &widget_portrait::on_inspiration_click);

}

void widget_portrait::on_inspiration_click()
{
	update_inspiration(! inspired);
	on_inspiration_event.emit(inspired);
}

void widget_portrait::update_inspiration(bool inspired)
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
