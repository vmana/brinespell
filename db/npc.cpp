#include "npc.h"
#include "yaml.h"

npc::npc()
{
}

npc::npc(string filename)
{
	yaml data;
	data.load(global::data_path + filename);

	name = data.value("name");

	token = data.value("token");
	if (token == "") token = "/data/" + filename + ".png";

	strength = convert::string_int(data.value("stats/str")); if (strength == 0) strength = 10;
	dexterity = convert::string_int(data.value("stats/dex")); if (dexterity == 0) dexterity = 10;
	constitution = convert::string_int(data.value("stats/con")); if (constitution == 0) constitution = 10;
	intelligence = convert::string_int(data.value("stats/int")); if (intelligence == 0) intelligence = 10;
	wisdom = convert::string_int(data.value("stats/wis")); if (wisdom == 0) wisdom = 10;
	charisma = convert::string_int(data.value("stats/cha")); if (charisma == 0) charisma = 10;
}

int npc::bonus_str() const
{
	return floor((double)(strength + tmp_strength - 10) / 2);
}

int npc::bonus_dex() const
{
	return floor((double)(dexterity + tmp_dexterity - 10) / 2);
}

int npc::bonus_con() const
{
	return floor((double)(constitution + tmp_constitution - 10) / 2);
}

int npc::bonus_int() const
{
	return floor((double)(intelligence + tmp_intelligence - 10) / 2);
}

int npc::bonus_cha() const
{
	return floor((double)(charisma + tmp_charisma - 10) / 2);
}

int npc::bonus_wis() const
{
	return floor((double)(wisdom + tmp_wisdom - 10) / 2);
}

