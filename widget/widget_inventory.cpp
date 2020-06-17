#include "widget_inventory.h"
#include "soma.h"
#include "db/player.h"

widget_inventory::widget_inventory() : wcontainer("character/inventory")
{
	setStyleClass("visibility_hidden");

	close = bindNew<WText>("close");
	close->setStyleClass("close");

	items = bindNew<WTextArea>("items");

	// update values
	update_values();

	// signal binding
	close->clicked().connect(this, &widget_inventory::switch_visibility);
	items->changed().connect(this, &widget_inventory::save_values);
	/* items->keyWentDown().connect([&](){ doJavaScript("resize_textarea('" + items->id() + "');"); }); */
}

void widget_inventory::update_values()
{
	dbo_session session;
	items->setText(S->p_player->p_inventory->items);
}

void widget_inventory::save_values()
{
	dbo_session session;
	S->p_player->p_inventory.modify()->items = items->text().toUTF8();
	update_values();
}

void widget_inventory::switch_visibility()
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
