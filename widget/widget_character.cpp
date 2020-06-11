#include "widget_character.h"
#include "soma.h"
#include "db/player.h"
#include "db/campaign.h"

widget_character::widget_character() : wcontainer("character/character")
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
	button_inspiration = bindNew<wtemplate>("button_inspiration", "character/ring_button");
	button_inspiration->setStyleClass("position_ring_inspiration");
	button_inspiration_icon = button_inspiration->bindNew<WText>("ring_button_icon");
	button_inspiration_helper = button_inspiration->bindNew<widget_template>("ring_button_helper");
	button_inspiration_helper->set_text("<div class=\"ring_button_helper_left\">Inspiration</div>");

	// initiative
	button_initiative = bindNew<wtemplate>("button_initiative", "character/ring_text_button");
	button_initiative->setStyleClass("position_ring_initiative");
	button_initiative_icon = button_initiative->bindNew<WText>("ring_button_icon");
	button_initiative_icon->setStyleClass("icon");
	initiative = button_initiative->bindNew<WLineEdit>("ring_button_text");
	button_initiative_helper = button_initiative->bindNew<widget_template>("ring_button_helper");
	button_initiative_helper->set_text("<div class=\"ring_button_helper_right\">Initiative</div>");

	// health bar
	health_bar = bindNew<wtemplate>("health_bar", "character/health_bar");
	health_bar->setStyleClass("position_health_bar");
	current_health_bar = health_bar->bindNew<WText>("current_health_bar");
	current_health_bar->setStyleClass("current_health_bar");
	health_bar_helper = health_bar->bindNew<widget_template>("ring_button_helper");
	health_bar_helper->setToolTip("Scroll to change Hit Points");

	// level
	button_level = bindNew<wtemplate>("button_level", "character/ring_button");
	button_level->setStyleClass("position_ring_level");
	button_level_text = button_level->bindNew<WText>("ring_button_icon");
	button_level_text->setStyleClass("text noselect");
	button_level_helper = button_level->bindNew<widget_template>("ring_button_helper");
	button_level_helper->set_text("<div class=\"ring_button_helper_right\">Character</div>");
	button_level_text->setText("6");

	// details hp
	details_hp = bindNew<widget_details_hp>("details_hp");

	// stats
	stats = bindNew<widget_stats>("stats");

	// signal binding
	button_inspiration->clicked().connect(this, &widget_character::on_inspiration_click);
	button_initiative->mouseWheel().connect(this, &widget_character::on_initiative_wheel);
	initiative->changed().connect(this, &widget_character::on_initiative_change);
	health_bar->mouseWheel().connect(this, &widget_character::on_health_bar_wheel);
	details_hp->hit_point_event.connect(this, &widget_character::update_health_bar);
	health_bar->clicked().connect(details_hp, &widget_details_hp::switch_details_hp_visibility);

	// update values
	update_inspiration();
	update_initiative();
	details_hp->update_hit_point();
}

void widget_character::on_inspiration_click()
{
	bool inspired = ! S->p_player->inspiration;
	// update db
	{
		dbo_session session;
		S->p_player.modify()->inspiration = inspired;
	}
	on_inspiration_event.emit(inspired);
	update_inspiration();
}

void widget_character::update_inspiration()
{
	bool inspired = S->p_player->inspiration;
	if (inspired)
	{
		button_inspiration_icon->setStyleClass("ring_button_icon ring_inspiration_sun");
	}
	else
	{
		button_inspiration_icon->setStyleClass("ring_button_icon ring_inspiration_moon");
	}
}

void widget_character::on_initiative_change()
{
	dbo_session session;
	int init = convert::string_int(initiative->text().toUTF8());
	if (init < 0)
	{
		// set old value
		initiative->setText(convert::int_string(S->p_player->initiative));
		return;
	}

	S->p_player.modify()->initiative = init;
	update_initiative();
}

void widget_character::on_initiative_wheel(const WMouseEvent &e)
{
	dbo_session session;
	if (e.wheelDelta() > 0) S->p_player.modify()->initiative += 1; // scroll up
	else if (e.wheelDelta() < 0) S->p_player.modify()->initiative -= 1; // scroll down
	update_initiative();
}

void widget_character::update_initiative()
{
	initiative->setText(convert::int_string(S->p_player->initiative));
}

void widget_character::on_health_bar_wheel(const WMouseEvent &e)
{
	dbo_session session;
	if (e.wheelDelta() > 0) S->p_player.modify()->wound(-1); // scroll up
	else if (e.wheelDelta() < 0) S->p_player.modify()->wound(1); // scroll down
	details_hp->update_hit_point();
}

void widget_character::update_health_bar(int percent, string helper)
{
	current_health_bar->setHeight(string(convert::int_string(percent) + "%"));

	health_bar_helper->set_text("<div class=\"ring_button_helper_right\">"
	+ helper
	+ "</div>");
}

