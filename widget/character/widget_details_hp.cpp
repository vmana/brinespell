#include "widget_details_hp.h"
#include "soma.h"
#include "db/player.h"

widget_details_hp::widget_details_hp() : wcontainer("character/details_hp")
{
	setStyleClass("visibility_hidden");

	close = bindNew<WText>("close");
	close->setStyleClass("close");
	max_hit_points = bindNew<WText>("max_hit_points");
	damage = bindNew<WLineEdit>("damage");
	tmp_hit_points = bindNew<WLineEdit>("tmp_hit_points");
	hit_points = bindNew<WText>("hit_points");

	// signal binding
	damage->changed().connect(this, &widget_details_hp::on_hp_change);
	damage->mouseWheel().connect(this, &widget_details_hp::on_hp_wheel);
	tmp_hit_points->changed().connect(this, &widget_details_hp::on_tmp_hp_change);
	tmp_hit_points->mouseWheel().connect(this, &widget_details_hp::on_tmp_hp_wheel);
	close->clicked().connect(this, &widget_details_hp::switch_visibility);
}

void widget_details_hp::update_hit_point()
{
	dbo_session session;

	// health bar
	int max_hp = S->p_player->max_hit_points();
	int total_hp = S->p_player->total_hit_points();

	// details hp
	max_hit_points->setText(convert::int_string(max_hp));
	hit_points->setText(convert::int_string(total_hp));
	damage->setText(convert::int_string(S->p_player->damage));
	tmp_hit_points->setText(convert::int_string(S->p_player->tmp_hit_points));

	int percent = 0;
	if (max_hp > 0) percent = (100 * total_hp) / max_hp;
	string helper = convert::int_string(total_hp) + " / " + convert::int_string(max_hp);

	// emit event to change character health bar
	hit_point_event.emit(percent, helper);
}

void widget_details_hp::on_hp_change()
{
	dbo_session session;
	int dmg = convert::string_int(damage->text().toUTF8());
	if (dmg < 0)
	{
		// set old value
		damage->setText(convert::int_string(S->p_player->damage));
		return;
	}

	S->p_player.modify()->set_damage(dmg);
	update_hit_point();
}

void widget_details_hp::on_hp_wheel(const WMouseEvent &e)
{
	dbo_session session;
	if (e.wheelDelta() > 0) S->p_player.modify()->wound(1); // scroll up
	else if (e.wheelDelta() < 0) S->p_player.modify()->wound(-1); // scroll down
	update_hit_point();
}

void widget_details_hp::on_tmp_hp_change()
{
	dbo_session session;
	int tmp_hp = convert::string_int(tmp_hit_points->text().toUTF8());
	if (tmp_hp < 0)
	{
		// set old value
		damage->setText(convert::int_string(S->p_player->tmp_hit_points));
		return;
	}

	S->p_player.modify()->tmp_hit_points = tmp_hp;
	update_hit_point();
}

void widget_details_hp::on_tmp_hp_wheel(const WMouseEvent &e)
{
	dbo_session session;
	if (e.wheelDelta() > 0) S->p_player.modify()->tmp_hit_points += 1; // scroll up
	else if (e.wheelDelta() < 0 && S->p_player->tmp_hit_points > 0) S->p_player.modify()->tmp_hit_points -= 1; // scroll down
	update_hit_point();
}

void widget_details_hp::switch_visibility()
{
	if (hasStyleClass("animate_show"))
	{
		removeStyleClass("visibility_hidden");
		setStyleClass("animate_hide");
	}
	else
	{
		setStyleClass("animate_show");
	}
}
