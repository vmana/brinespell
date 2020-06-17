#include "widget_notes.h"
#include "soma.h"
#include "db/player.h"

widget_notes::widget_notes() : wcontainer("character/notes")
{
	setStyleClass("visibility_hidden");

	close = bindNew<WText>("close");
	close->setStyleClass("close");

	my_notes = bindNew<WTextArea>("my_notes");

	// update values
	update_values();

	// signal binding
	close->clicked().connect(this, &widget_notes::switch_visibility);
	my_notes->changed().connect(this, &widget_notes::save_values);
	/* items->keyWentDown().connect([&](){ doJavaScript("resize_textarea('" + items->id() + "');"); }); */
}

void widget_notes::update_values()
{
	dbo_session session;
	my_notes->setText(S->p_player->p_notes->my_notes);
}

void widget_notes::save_values()
{
	dbo_session session;
	S->p_player->p_notes.modify()->my_notes = my_notes->text().toUTF8();
	update_values();
}

void widget_notes::switch_visibility()
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
