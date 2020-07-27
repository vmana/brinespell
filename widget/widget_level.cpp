#include "widget_level.h"
#include "soma.h"
#include "db/player.h"

widget_level::widget_level() : wcontainer("character/level")
{
	setStyleClass("visibility_hidden");

	close = bindNew<WText>("close");
	close->setStyleClass("close");

	level = bindNew<WLineEdit>("level");
	level->setStyleClass("level");
	level->setToolTip("Level");

	character_name = bindNew<WLineEdit>("character_name");
	character_name->setStyleClass("character_name");
	character_name->setPlaceholderText("Character name");

	character_class = bindNew<WLineEdit>("character_class");
	character_class->setStyleClass("character_class");
	character_class->setPlaceholderText("Class");

	race = bindNew<WLineEdit>("race");
	race->setStyleClass("race");
	race->setPlaceholderText("Race");

	// left column stats
	proficiency = bindNew<WText>("proficiency");
	strength = bindNew<WLineEdit>("strength");
	dexterity = bindNew<WLineEdit>("dexterity");
	constitution = bindNew<WLineEdit>("constitution");
	intelligence = bindNew<WLineEdit>("intelligence");
	wisdom = bindNew<WLineEdit>("wisdom");
	charisma = bindNew<WLineEdit>("charisma");
	armor_class = bindNew<WLineEdit>("armor_class");
	class_hit_points = bindNew<WLineEdit>("class_hit_points");
	speed = bindNew<WLineEdit>("speed");

	features = bindNew<WTextArea>("features");

	// update values
	update_values();

	doJavaScript("w_scrollarea('" + features->id() + "');");

	// signal binding
	close->clicked().connect(this, &widget_level::switch_visibility);

	character_name->changed().connect(this, &widget_level::save_values);
	level->changed().connect(this, &widget_level::save_values);
	character_class->changed().connect(this, &widget_level::save_values);
	race->changed().connect(this, &widget_level::save_values);

	armor_class->changed().connect(this, &widget_level::save_values);
	class_hit_points->changed().connect(this, &widget_level::save_values);
	speed->changed().connect(this, &widget_level::save_values);

	strength->changed().connect(this, &widget_level::save_values);
	dexterity->changed().connect(this, &widget_level::save_values);
	constitution->changed().connect(this, &widget_level::save_values);
	intelligence->changed().connect(this, &widget_level::save_values);
	wisdom->changed().connect(this, &widget_level::save_values);
	charisma->changed().connect(this, &widget_level::save_values);

	features->changed().connect(this, &widget_level::save_values);
}

void widget_level::update_values()
{
	dbo_session session;
	level->setText(convert::int_string(S->p_player->level));
	character_name->setText(S->p_player->name);
	character_class->setText(S->p_player->character_class);
	race->setText(S->p_player->race);
	proficiency->setText(convert::int_string(S->p_player->proficiency()));
	strength->setText(convert::int_string(S->p_player->strength));
	dexterity->setText(convert::int_string(S->p_player->dexterity));
	constitution->setText(convert::int_string(S->p_player->constitution));
	intelligence->setText(convert::int_string(S->p_player->intelligence));
	wisdom->setText(convert::int_string(S->p_player->wisdom));
	charisma->setText(convert::int_string(S->p_player->charisma));
	armor_class->setText(convert::int_string(S->p_player->armor_class));
	class_hit_points->setText(convert::int_string(S->p_player->class_hit_points));
	speed->setText(convert::int_string(S->p_player->speed));
	features->setText(S->p_player->features);
}

void widget_level::save_values()
{
	dbo_session session;
	S->p_player.modify()->name = character_name->text().toUTF8();
	S->p_player.modify()->level = abs(convert::string_int(level->text().toUTF8()));
	S->p_player.modify()->character_class = character_class->text().toUTF8();
	S->p_player.modify()->race = race->text().toUTF8();

	S->p_player.modify()->armor_class = abs(convert::string_int(armor_class->text().toUTF8()));
	S->p_player.modify()->class_hit_points = abs(convert::string_int(class_hit_points->text().toUTF8()));
	S->p_player.modify()->speed = abs(convert::string_int(speed->text().toUTF8()));

	S->p_player.modify()->strength = abs(convert::string_int(strength->text().toUTF8()));
	S->p_player.modify()->dexterity = abs(convert::string_int(dexterity->text().toUTF8()));
	S->p_player.modify()->constitution = abs(convert::string_int(constitution->text().toUTF8()));
	S->p_player.modify()->intelligence = abs(convert::string_int(intelligence->text().toUTF8()));
	S->p_player.modify()->wisdom = abs(convert::string_int(wisdom->text().toUTF8()));
	S->p_player.modify()->charisma = abs(convert::string_int(charisma->text().toUTF8()));

	S->p_player.modify()->features = features->text().toUTF8();

	// emit signal for parent
	on_change.emit();

	update_values();
}

void widget_level::switch_visibility()
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
