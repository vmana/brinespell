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

	// inspiration
	button_inspiration = bindNew<wtemplate>("button_inspiration", "ring_button");
	button_inspiration->setStyleClass("position_ring_inspiration");
	button_inspiration_bg = button_inspiration->bindNew<WText>("ring_button_bg");
	button_inspiration_helper = button_inspiration->bindNew<widget_template>("ring_button_helper");
	button_inspiration_helper->set_text("<div class=\"ring_button_helper_left\">Inspiration</div>");

	// health bar
	health_bar = bindNew<wtemplate>("health_bar", "health_bar");
	health_bar->setStyleClass("position_health_bar");
	current_health_bar = health_bar->bindNew<WText>("current_health_bar");
	current_health_bar->setStyleClass("current_health_bar");
	health_bar_helper = health_bar->bindNew<widget_template>("ring_button_helper");
	health_bar_helper->setToolTip("Scroll to change Hit Points");

	// details hp
	details_hp = bindNew<wtemplate>("details_hp", "details_hp");
	details_hp->setStyleClass("visibility_hidden");

	close_details_hp = details_hp->bindNew<WText>("close");
	close_details_hp->setStyleClass("close");
	details_max_hit_points = details_hp->bindNew<WText>("max_hit_points");
	details_damage = details_hp->bindNew<WLineEdit>("damage");
	details_tmp_hit_points = details_hp->bindNew<WLineEdit>("tmp_hit_points");
	details_hit_points = details_hp->bindNew<WText>("hit_points");

	// update values from database
	inspired = S->p_player->inspiration;

	update_inspiration(inspired);
	update_hit_point();

	// signal binding
	button_inspiration->clicked().connect(this, &widget_character::on_inspiration_click);
	health_bar->mouseWheel().connect(this, &widget_character::on_health_bar_wheel);
	health_bar->clicked().connect(this, &widget_character::switch_details_hp_visibility);
	close_details_hp->clicked().connect(this, &widget_character::switch_details_hp_visibility);

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

void widget_character::on_health_bar_wheel(const WMouseEvent &e)
{
	dbo_session session;
	if (e.wheelDelta() > 0)
	{
		// scroll up
		S->p_player.modify()->set_damage(-1);
	}
	else if (e.wheelDelta() < 0)
	{
		// scroll down
		S->p_player.modify()->set_damage(1);
	}
	update_hit_point();
}

void widget_character::update_hit_point()
{
	dbo_session session;
	int percent = 0;

	// health bar
	int max_hp = S->p_player->max_hit_points();
	int total_hp = S->p_player->total_hit_points();

	if (max_hp > 0) percent = (100 * total_hp) / max_hp;
	current_health_bar->setHeight(string(convert::int_string(percent) + "%"));

	health_bar_helper->set_text("<div class=\"ring_button_helper_right\">"
	+ convert::int_string(total_hp) + " / " + convert::int_string(max_hp)
	+ "</div>");

	// details hp
	details_max_hit_points->setText(convert::int_string(max_hp));
	details_hit_points->setText(convert::int_string(total_hp));
	details_damage->setText(convert::int_string(S->p_player->damage));
	details_tmp_hit_points->setText(convert::int_string(S->p_player->tmp_hit_points));
}

void widget_character::switch_details_hp_visibility()
{
	if (details_hp->hasStyleClass("animate_show"))
	{
		details_hp->removeStyleClass("visibility_hidden");
		details_hp->setStyleClass("animate_hide");
	}
	else
	{
		details_hp->setStyleClass("animate_show");
	}
}
