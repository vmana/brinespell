#include "player.h"
#include "soma_database.h"
#include "campaign.h"

player::player()
{
}

int player::proficiency() const
{
	return ceil((double)level / 4 + 1);
}

int player::bonus_str() const
{
	return floor((double)(strength + tmp_strength - 10) / 2);
}

int player::bonus_dex() const
{
	return floor((double)(dexterity + tmp_dexterity - 10) / 2);
}

int player::bonus_con() const
{
	return floor((double)(constitution + tmp_constitution - 10) / 2);
}

int player::bonus_int() const
{
	return floor((double)(intelligence + tmp_intelligence - 10) / 2);
}

int player::bonus_cha() const
{
	return floor((double)(charisma + tmp_charisma - 10) / 2);
}

int player::bonus_wis() const
{
	return floor((double)(wisdom + tmp_wisdom - 10) / 2);
}

int player::max_hit_points() const
{
	int ret = class_hit_points + tmp_hit_points + bonus_con() * level;
	if (ret < 0) ret = 0;
	return ret;
}

int player::total_hit_points() const
{
	int ret = max_hit_points() - damage;
	if (ret < 0) ret = 0;
	return ret;
}

void player::wound(int dmg)
{
	if (dmg < 0)
	{
		damage += dmg; // will reduce damage
		if (damage < 0) damage = 0;
	}
	else
	{
		// wound player
		tmp_hit_points -= dmg;
		if (tmp_hit_points < 0)
		{
			// damage left to real damage
			damage += - tmp_hit_points;
			tmp_hit_points = 0;
		}
		int max_hp = max_hit_points();
		if (damage > max_hp) damage = max_hp;
	}
}

void player::set_damage(int dmg)
{
	int delta = dmg - damage;
	if (delta > 0)
	{
		// more damage, try to reduce tmp_hp
		tmp_hit_points -= delta;
		if (tmp_hit_points < 0)
		{
			// damage left to real damage
			delta = - tmp_hit_points;
			tmp_hit_points = 0;
		}
	}

	// add delta, and adjust value if out of range
	int max_hp = max_hit_points();
	damage += delta;
	if (damage < 0) damage = 0;
	else if (damage > max_hp) damage = max_hp;
}

string player::avatar_image() const
{
	string filename = "/data/campaign/" + strlower(p_campaign->name) + "/avatar/";
	if (avatar != "") filename += avatar + ".png";
	else filename += strlower(name) + ".png";
	return filename;
}

string player::token_image() const
{
	string filename = "campaign/" + strlower(p_campaign->name) + "/avatar/";
	if (token != "") filename += token + ".png";
	else filename += "token_" + strlower(name) + ".png";
	// check if it exists, if not the show its avatar
	if (!file::exists(global::data_path + filename)) return avatar_image();

	return "/data/" + filename;
}
