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
	button_inspiration = bindNew<wtemplate>("inspiration", "ring_button");
	button_inspiration->setStyleClass("position_ring_inspiration");
	button_inspiration_bg = button_inspiration->bindNew<WText>("ring_button_bg");
	button_inspiration_helper = button_inspiration->bindNew<widget_template>("ring_button_helper");
	button_inspiration_helper->set_text("<div class=\"ring_button_helper_left\">Inspiration</div>");

	// initiative
	button_initiative = bindNew<wtemplate>("initiative", "ring_text_button");
	button_initiative->setStyleClass("position_ring_initiative");
	button_initiative_bg = button_initiative->bindNew<WText>("ring_button_bg");
	button_initiative_bg->setStyleClass("ring_initiative_wind");
	initiative = button_initiative->bindNew<WLineEdit>("ring_button_text");
	button_initiative_helper = button_initiative->bindNew<widget_template>("ring_button_helper");
	button_initiative_helper->set_text("<div class=\"ring_button_helper_right\">Initiative</div>");

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

	update_inspiration();
	update_initiative();
	update_hit_point();

	// signal binding
	button_inspiration->clicked().connect(this, &widget_character::on_inspiration_click);

	button_initiative->clicked().connect(this, &widget_character::on_initiative_click);
	button_initiative->mouseWheel().connect(this, &widget_character::on_initiative_wheel);
	initiative->changed().connect(this, &widget_character::on_initiative_change);

	health_bar->mouseWheel().connect(this, &widget_character::on_health_bar_wheel);
	health_bar->clicked().connect(this, &widget_character::switch_details_hp_visibility);
	details_damage->changed().connect(this, &widget_character::on_details_hp_change);
	details_damage->mouseWheel().connect(this, &widget_character::on_details_hp_wheel);
	details_tmp_hit_points->changed().connect(this, &widget_character::on_details_tmp_hp_change);
	details_tmp_hit_points->mouseWheel().connect(this, &widget_character::on_details_tmp_hp_wheel);
	close_details_hp->clicked().connect(this, &widget_character::switch_details_hp_visibility);

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
		button_inspiration_bg->setStyleClass("ring_button_bg ring_inspiration_sun");
	}
	else
	{
		button_inspiration_bg->setStyleClass("ring_button_bg ring_inspiration_moon");
	}
}

void widget_character::on_initiative_click()
{
	initiative->setFocus(true);
	initiative->setSelection(0, initiative->text().toUTF8().length());
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

void widget_character::on_details_hp_change()
{
	dbo_session session;
	int dmg = convert::string_int(details_damage->text().toUTF8());
	if (dmg < 0)
	{
		// set old value
		details_damage->setText(convert::int_string(S->p_player->damage));
		return;
	}

	S->p_player.modify()->set_damage(dmg);
	update_hit_point();
}

void widget_character::on_details_hp_wheel(const WMouseEvent &e)
{
	dbo_session session;
	if (e.wheelDelta() > 0) S->p_player.modify()->wound(1); // scroll up
	else if (e.wheelDelta() < 0) S->p_player.modify()->wound(-1); // scroll down
	update_hit_point();
}

void widget_character::on_details_tmp_hp_change()
{
	dbo_session session;
	int tmp_hp = convert::string_int(details_tmp_hit_points->text().toUTF8());
	if (tmp_hp < 0)
	{
		// set old value
		details_damage->setText(convert::int_string(S->p_player->tmp_hit_points));
		return;
	}

	S->p_player.modify()->tmp_hit_points = tmp_hp;
	update_hit_point();
}

void widget_character::on_details_tmp_hp_wheel(const WMouseEvent &e)
{
	dbo_session session;
	if (e.wheelDelta() > 0) S->p_player.modify()->tmp_hit_points += 1; // scroll up
	else if (e.wheelDelta() < 0) S->p_player.modify()->tmp_hit_points -= 1; // scroll down
	update_hit_point();
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
