#include "widget_stats.h"
#include "soma.h"
#include "db/player.h"
#include "db/skill.h"

widget_stats::widget_stats() : wcontainer("character/stats")
{
	setStyleClass("widget_stats");

	auto button_str = bindNew<wtemplate>("button_str", "character/stat_button");
	button_str->setStyleClass("div_stats_item");
	mod_str = button_str->bindNew<WText>("text");
	button_str->bindString("stat", "strength");

	auto button_dex = bindNew<wtemplate>("button_dex", "character/stat_button");
	button_dex->setStyleClass("div_stats_item");
	mod_dex = button_dex->bindNew<WText>("text");
	button_dex->bindString("stat", "dexterity");

	auto button_con = bindNew<wtemplate>("button_con", "character/stat_button");
	button_con->setStyleClass("div_stats_item");
	mod_con = button_con->bindNew<WText>("text");
	button_con->bindString("stat", "constitution");

	auto button_int = bindNew<wtemplate>("button_int", "character/stat_button");
	button_int->setStyleClass("div_stats_item");
	mod_int = button_int->bindNew<WText>("text");
	button_int->bindString("stat", "intelligence");

	auto button_wis = bindNew<wtemplate>("button_wis", "character/stat_button");
	button_wis->setStyleClass("div_stats_item");
	mod_wis = button_wis->bindNew<WText>("text");
	button_wis->bindString("stat", "wisdom");

	auto button_cha = bindNew<wtemplate>("button_cha", "character/stat_button");
	button_cha->setStyleClass("div_stats_item");
	mod_cha = button_cha->bindNew<WText>("text");
	button_cha->bindString("stat", "charisma");

	details_stat = bindNew<wtemplate>("details_stat", "character/details_stat");
	details_stat->setStyleClass("visibility_hidden");

	button_str->clicked().connect(bind(&widget_stats::prepare_details, this, "Strength"));
	button_dex->clicked().connect(bind(&widget_stats::prepare_details, this, "Dexterity"));
	button_con->clicked().connect(bind(&widget_stats::prepare_details, this, "Constitution"));
	button_int->clicked().connect(bind(&widget_stats::prepare_details, this, "Intelligence"));
	button_wis->clicked().connect(bind(&widget_stats::prepare_details, this, "Wisdom"));
	button_cha->clicked().connect(bind(&widget_stats::prepare_details, this, "Charisma"));

	update_stats();
}

string widget_stats::notation(int value)
{
	string ret = "";
	if (value >= 0) ret = "+";
	ret += convert::int_string(value);
	return ret;
}

void widget_stats::update_stats()
{
	mod_str->setText(notation(S->p_player->bonus_str()));
	mod_dex->setText(notation(S->p_player->bonus_dex()));
	mod_con->setText(notation(S->p_player->bonus_con()));
	mod_int->setText(notation(S->p_player->bonus_int()));
	mod_wis->setText(notation(S->p_player->bonus_wis()));
	mod_cha->setText(notation(S->p_player->bonus_cha()));
}

void widget_stats::prepare_details(string name)
{
	if (name == current_stat)
	{
		// hide
		switch_details_visibility();
		return;
	}

	current_stat = name;
	details_stat->load("character/details_stat");

	close_stat = details_stat->bindNew<WText>("close");
	close_stat->setStyleClass("close");

	details_stat->bindString("name_stat", current_stat);
	base_stat = details_stat->bindNew<WText>("base_stat");
	tmp_stat = details_stat->bindNew<WLineEdit>("tmp_stat");
	total_stat = details_stat->bindNew<WText>("total_stat");
	mod_stat = details_stat->bindNew<WText>("mod_stat");
	details_skill = details_stat->bindNew<wtemplate>("details_skill");
	details_skill->setStyleClass("flex_skill");


	if (current_stat == "Strength")
	{
		details_stat->bindString("icon_stat", "sword");
	}
	else if (current_stat == "Dexterity")
	{
		details_stat->bindString("icon_stat", "cat");
	}
	else if (current_stat == "Constitution")
	{
		details_stat->bindString("icon_stat", "heart");
	}
	else if (current_stat == "Intelligence")
	{
		details_stat->bindString("icon_stat", "open-book");
	}
	else if (current_stat == "Wisdom")
	{
		details_stat->bindString("icon_stat", "eye");
	}
	else if (current_stat == "Charisma")
	{
		details_stat->bindString("icon_stat", "stars");
	}

	update_details();

	/****    create skills    ****/

	dbo_session session;
	string query =
		"select s from skill s, base_skill bs where s.base_skill_id = bs.id" \
		" and bs.attribute = ? and s.player_id = ?";
	dbo::collection<dbo::ptr<skill>> skills =
		session->query<dbo::ptr<skill>>(query)
		.bind(current_stat)
		.bind(S->p_player.id());

	// clear previous
	vskill.clear();

	for (auto p_skill : skills)
	{
		auto new_line = make_unique<wtemplate>("character/skill");
		new_line->setStyleClass("flex_skill_line");

		auto skill_proficiency = new_line->bindNew<WImage>("proficiency", "/img/check-off.svg");
		skill_proficiency->setStyleClass("icon");
		new_line->bindString("name", p_skill->p_base_skill->name);
		auto skill_total = new_line->bindNew<WLineEdit>("total");

		skill_line vline = {p_skill, skill_proficiency, skill_total};
		update_skill(vline);

		// signal binding
		skill_total->changed().connect(bind(&widget_stats::on_skill_changed, this, vline));
		skill_proficiency->clicked().connect(bind(&widget_stats::on_proficiency_click, this, vline));

		vskill.push_back(vline);
		*details_skill += new_line;
	}

	// signal binding
	close_stat->clicked().connect(this, &widget_stats::switch_details_visibility);
	tmp_stat->changed().connect(this, &widget_stats::on_tmp_stat_changed);

	details_stat->setStyleClass("animate_show");
}

void widget_stats::update_details()
{
	if (current_stat == "Strength")
	{
		base_stat->setText(convert::int_string(S->p_player->strength));
		tmp_stat->setText(convert::int_string(S->p_player->tmp_strength));
		total_stat->setText(convert::int_string(S->p_player->strength + S->p_player->tmp_strength));
		mod_stat->setText(convert::int_string(S->p_player->bonus_str()));
	}
	else if (current_stat == "Dexterity")
	{
		base_stat->setText(convert::int_string(S->p_player->dexterity));
		tmp_stat->setText(convert::int_string(S->p_player->tmp_dexterity));
		total_stat->setText(convert::int_string(S->p_player->dexterity + S->p_player->tmp_dexterity));
		mod_stat->setText(convert::int_string(S->p_player->bonus_dex()));
	}
	else if (current_stat == "Constitution")
	{
		base_stat->setText(convert::int_string(S->p_player->constitution));
		tmp_stat->setText(convert::int_string(S->p_player->tmp_constitution));
		total_stat->setText(convert::int_string(S->p_player->constitution + S->p_player->tmp_constitution));
		mod_stat->setText(convert::int_string(S->p_player->bonus_con()));
	}
	else if (current_stat == "Intelligence")
	{
		base_stat->setText(convert::int_string(S->p_player->intelligence));
		tmp_stat->setText(convert::int_string(S->p_player->tmp_intelligence));
		total_stat->setText(convert::int_string(S->p_player->intelligence + S->p_player->tmp_intelligence));
		mod_stat->setText(convert::int_string(S->p_player->bonus_int()));
	}
	else if (current_stat == "Wisdom")
	{
		base_stat->setText(convert::int_string(S->p_player->wisdom));
		tmp_stat->setText(convert::int_string(S->p_player->tmp_wisdom));
		total_stat->setText(convert::int_string(S->p_player->wisdom + S->p_player->tmp_wisdom));
		mod_stat->setText(convert::int_string(S->p_player->bonus_wis()));
	}
	else if (current_stat == "Charisma")
	{
		base_stat->setText(convert::int_string(S->p_player->charisma));
		tmp_stat->setText(convert::int_string(S->p_player->tmp_charisma));
		total_stat->setText(convert::int_string(S->p_player->charisma + S->p_player->tmp_charisma));
		mod_stat->setText(convert::int_string(S->p_player->bonus_cha()));
	}
}

void widget_stats::on_tmp_stat_changed()
{
	dbo_session session;

	int value = convert::string_int(tmp_stat->text().toUTF8());
	if (current_stat == "Strength") S->p_player.modify()->tmp_strength = value;
	else if (current_stat == "Dexterity") S->p_player.modify()->tmp_dexterity = value;
	else if (current_stat == "Constitution") S->p_player.modify()->tmp_constitution = value;
	else if (current_stat == "Intelligence") S->p_player.modify()->tmp_intelligence = value;
	else if (current_stat == "Wisdom") S->p_player.modify()->tmp_wisdom = value;
	else if (current_stat == "Charisma") S->p_player.modify()->tmp_charisma = value;
	update_details();
	update_stats();
	update_skills();
}

void widget_stats::update_skills()
{
	// update all skills
	for (auto &line : vskill)
		update_skill(line);
}

void widget_stats::update_skill(skill_line &line)
{
	auto &[p_skill, img, total] = line;
	dbo_session session;
	int value = convert::string_int(mod_stat->text().toUTF8());
	string tooltip = "= " + mod_stat->text().toUTF8() + " (stat modifier) + ";

	value += p_skill->bonus;

	// proficiency image + bonus
	if (p_skill->proficient)
	{
		value += p_skill->p_player->proficiency();
		tooltip += convert::int_string(p_skill->p_player->proficiency());
		img->setImageLink("/img/check-on.svg");
	}
	else
	{
		img->setImageLink("/img/check-off.svg");
		tooltip += "0";
	}

	tooltip += " (proficiency) + " + convert::int_string(p_skill->bonus) + " (bonus)";
	total->setToolTip(tooltip);

	total->setText(convert::int_string(value));
}

void widget_stats::on_skill_changed(skill_line &line)
{
	// determine bonus value, based on proficiency & base stats
	auto &[p_skill, img, total] = line;
	dbo_session session;

	// if empty, set bonus to 0
	if (total->text().toUTF8() == "")
	{
		p_skill.modify()->bonus = 0;
		update_skill(line);
		return;
	}
	int sum = convert::string_int(total->text().toUTF8());
	int stat = convert::string_int(mod_stat->text().toUTF8());
	int prof = (p_skill->proficient) ? p_skill->p_player->proficiency() : 0;
	int bonus = sum - stat - prof;

	p_skill.modify()->bonus = bonus;
	update_skill(line);
}

void widget_stats::on_proficiency_click(skill_line &line)
{
	auto &[p_skill, img, total] = line;
	dbo_session session;

	p_skill.modify()->proficient = !p_skill->proficient;
	if (p_skill->proficient) img->setImageLink("/img/check-on.svg");
	else img->setImageLink("/img/check-off.svg");

	update_skill(line);
}

void widget_stats::switch_details_visibility()
{
	if (details_stat->hasStyleClass("animate_show"))
	{
		details_stat->removeStyleClass("visibility_hidden");
		details_stat->setStyleClass("animate_hide");
	}
	else
	{
		details_stat->setStyleClass("animate_show");
	}
}
