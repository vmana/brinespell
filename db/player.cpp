#include "player.h"
#include "soma_database.h"

player::player()
{
}

int player::bonus_str() const
{
	return floor((double)(strength - 10) / 2);
}

int player::bonus_dex() const
{
	return floor((double)(dexterity - 10) / 2);
}

int player::bonus_con() const
{
	return floor((double)(constitution - 10) / 2);
}

int player::bonus_int() const
{
	return floor((double)(intelligence - 10) / 2);
}

int player::bonus_cha() const
{
	return floor((double)(charisma - 10) / 2);
}

int player::bonus_wis() const
{
	return floor((double)(wisdom - 10) / 2);
}

int player::max_hit_points() const
{
	int ret = class_hit_points + tmp_hit_points + bonus_con();
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
