#include "widget_character.h"
#include "soma.h"
#include "db/player.h"
#include "db/campaign.h"

widget_character::widget_character() : wcontainer("character/character")
{
	setStyleClass("div_character");

	// css animations
	bindString("css_animations", (global::css_animations) ? "" : "no-animation");

	// set player character image
	avatar_image = bindNew<WText>("avatar_image");
	avatar_image->setStyleClass("avatar_image");
	avatar_image->setToolTip("Drag to create a token");

	drag_token = bindNew<widget_drag_token>("drag_token");
	// set current player, will be used on drop to determine if we already have a token
	drag_token->p_player = S->p_player;

	// allow drag & drop from avatar
	avatar_image->setDraggable("player_token", drag_token, true, drag_token);

	// inspiration
	button_inspiration = bindNew<wtemplate>("button_inspiration", "character/ring_button");
	button_inspiration->setStyleClass("position_ring_inspiration");
	button_inspiration->bindString("css_animations", (global::css_animations) ? "" : "no-animation");
	button_inspiration_icon = button_inspiration->bindNew<WText>("ring_button_icon");
	button_inspiration_helper = button_inspiration->bindNew<widget_template>("ring_button_helper");
	button_inspiration_helper->set_text("<div class=\"ring_button_helper_left\">Inspiration</div>");

	// initiative
	button_initiative = bindNew<wtemplate>("button_initiative", "character/ring_text_button");
	button_initiative->setStyleClass("position_ring_initiative");
	button_initiative->bindString("css_animations", (global::css_animations) ? "" : "no-animation");
	button_initiative_icon = button_initiative->bindNew<WText>("ring_button_icon");
	button_initiative_icon->setStyleClass("icon");
	initiative = button_initiative->bindNew<WLineEdit>("ring_button_text");
	button_initiative_helper = button_initiative->bindNew<widget_template>("ring_button_helper");
	button_initiative_helper->set_text("<div class=\"ring_button_helper_right\">Initiative</div>");

	// health bar
	health_bar = bindNew<wtemplate>("health_bar", "character/health_bar");
	health_bar->setStyleClass("position_health_bar");
	health_bar->bindString("css_animations", (global::css_animations) ? "" : "no-animation");
	current_health_bar = health_bar->bindNew<WText>("current_health_bar");
	current_health_bar->setStyleClass("current_health_bar");
	health_bar_helper = health_bar->bindNew<widget_template>("ring_button_helper");
	health_bar_helper->setToolTip("Scroll to change Hit Points");

	// level
	button_level = bindNew<wtemplate>("button_level", "character/ring_button");
	button_level->setStyleClass("position_ring_level");
	button_level->bindString("css_animations", (global::css_animations) ? "" : "no-animation");
	button_level_text = button_level->bindNew<WText>("ring_button_icon");
	button_level_text->setStyleClass("text noselect");
	button_level_helper = button_level->bindNew<widget_template>("ring_button_helper");
	button_level_helper->set_text("<div class=\"ring_button_helper_right\">Character</div>");

	// weapon
	button_weapon = bindNew<wtemplate>("button_weapon", "character/ring_button");
	button_weapon->setStyleClass("position_ring_weapon");
	button_weapon->bindString("css_animations", (global::css_animations) ? "" : "no-animation");
	button_weapon_icon = button_weapon->bindNew<WText>("ring_button_icon");
	button_weapon_icon->setStyleClass("ring_button_icon ring_weapon_icon");
	button_weapon_helper = button_weapon->bindNew<widget_template>("ring_button_helper");
	button_weapon_helper->set_text("<div class=\"ring_button_helper_left\">Weapon</div>");

	// armor class
	button_armor = bindNew<wtemplate>("button_armor", "character/shield");
	button_armor->setStyleClass("position_armor");
	button_armor->bindString("css_animations", (global::css_animations) ? "" : "no-animation");
	armor = button_armor->bindNew<WLineEdit>("armor_button_text");
	button_armor_helper = button_armor->bindNew<widget_template>("armor_button_helper");
	button_armor_helper->set_text("<div class=\"ring_button_helper_left\">Armor Class</div>");

	// inventory
	button_inventory = bindNew<wtemplate>("button_inventory", "character/ring_simple");
	button_inventory->setStyleClass("item position_ring_inventory");
	button_inventory->bindString("css_animations", (global::css_animations) ? "" : "no-animation");
	button_inventory_icon = button_inventory->bindNew<WText>("ring_button_icon");
	button_inventory_icon->setStyleClass("icon");

	// spellbook
	button_spellbook = bindNew<wtemplate>("button_spellbook", "character/ring_simple");
	button_spellbook->setStyleClass("item position_ring_spellbook");
	button_spellbook->bindString("css_animations", (global::css_animations) ? "" : "no-animation");
	button_spellbook_icon = button_spellbook->bindNew<WText>("ring_button_icon");
	button_spellbook_icon->setStyleClass("icon");

	// notes
	button_notes = bindNew<wtemplate>("button_notes", "character/ring_simple");
	button_notes->setStyleClass("item position_ring_notes");
	button_notes->bindString("css_animations", (global::css_animations) ? "" : "no-animation");
	button_notes_icon = button_notes->bindNew<WText>("ring_button_icon");
	button_notes_icon->setStyleClass("icon");

	// panels
	details_hp = bindNew<widget_details_hp>("details_hp");
	level = bindNew<widget_level>("level");
	stats = bindNew<widget_stats>("stats");
	weapon = bindNew<widget_weapon>("weapon");
	inventory = bindNew<widget_inventory>("inventory");
	notes = bindNew<widget_notes>("notes");

	// signal binding
	button_inspiration->clicked().connect(this, &widget_character::on_inspiration_click);
	button_initiative->mouseWheel().connect(this, &widget_character::on_initiative_wheel);
	initiative->changed().connect(this, &widget_character::on_initiative_change);
	health_bar->mouseWheel().connect(this, &widget_character::on_health_bar_wheel);
	details_hp->hit_point_event.connect(this, &widget_character::update_health_bar);
	health_bar->clicked().connect(details_hp, &widget_details_hp::switch_visibility);
	button_level->clicked().connect(level, &widget_level::switch_visibility);
	level->on_change.connect(this, &widget_character::on_character_level_change);
	button_weapon->clicked().connect(weapon, &widget_weapon::switch_visibility);
	button_weapon->mouseWheel().connect(this, &widget_character::on_weapon_wheel);
	weapon->on_change.connect(this, &widget_character::on_weapon_change);
	armor->changed().connect(this, &widget_character::on_armor_change);
	armor->mouseWheel().connect(this, &widget_character::on_armor_wheel);
	button_inventory->clicked().connect(inventory, &widget_inventory::switch_visibility);
	button_notes->clicked().connect(notes, &widget_notes::switch_visibility);

	// update values
	update_character();
}

void widget_character::update_character()
{
	update_image();
	update_inspiration();
	update_initiative();
	update_level();
	update_weapon();
	update_armor();
	details_hp->update_hit_point();
	stats->update_stats();
	stats->reload_details();
	level->update_values();
	inventory->update_values();
	notes->update_values();
}

void widget_character::update_image()
{
	avatar_image->decorationStyle().setBackgroundImage(S->p_player->avatar_image());
	string token_image = S->p_player->token_image();
	drag_token->filename = token_image;
	drag_token->decorationStyle().setBackgroundImage(token_image);
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

void widget_character::update_level()
{
	button_level_text->setText(convert::int_string(S->p_player->level));
}

void widget_character::on_character_level_change()
{
	update_level();
	update_armor();
	// update health bar
	details_hp->update_hit_point(); // will trigger an event with percentage & helper info
	// update stats modifiers
	stats->update_stats();
}

void widget_character::update_weapon()
{
	weapon->update_values(); // will trigger an event with helper values
}

void widget_character::on_weapon_change(int current_weapon, string helper)
{
	button_weapon_icon->setStyleClass("ring_button_icon ring_weapon_icon_" + convert::int_string(current_weapon));
	button_weapon_helper->set_text("<div class=\"ring_button_helper_left\">"+ helper + "</div>");
}

void widget_character::on_weapon_wheel(const WMouseEvent &e)
{
	dbo_session session;
	int current_weapon = S->p_player->p_inventory->current_weapon;
	if (e.wheelDelta() > 0) current_weapon -= 1; // scroll up
	else if (e.wheelDelta() < 0) current_weapon += 1; // scroll down

	// prevent out of range
	if (current_weapon < 0) current_weapon = 2;
	else if (current_weapon > 2) current_weapon = 0;
	weapon->on_current_weapon_click(current_weapon);
}

void widget_character::update_armor()
{
	armor->setText(convert::int_string(S->p_player->armor_class));
}

void widget_character::on_armor_change()
{
	dbo_session session;
	S->p_player.modify()->armor_class = abs(convert::string_int(armor->text().toUTF8()));
	level->update_values();
	update_armor();
}

void widget_character::on_armor_wheel(const WMouseEvent &e)
{
	dbo_session session;
	if (e.wheelDelta() > 0) S->p_player.modify()->armor_class += 1; // scroll up
	else if (e.wheelDelta() < 0 && S->p_player->armor_class > 0) S->p_player.modify()->armor_class -= 1; // scroll down
	level->update_values();
	update_armor();
}
