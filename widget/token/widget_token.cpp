#include "widget_token.h"
#include "soma.h"

widget_token::widget_token(string filename, bool visible) :
	widget_token(filename, mana::randstring(16), visible)
{
}

widget_token::widget_token(string filename, string id, bool visible) :
	widget_token(filename, id, 220, 550, visible)
{
}

widget_token::widget_token(string filename, string id, int x, int y, bool visible) :
	wcontainer("token/player"),
	signal_move(this, "signal_move")
{
	setStyleClass("widget_token");

	setId(id);
	change_token_visibility(visible);

	setPositionScheme(PositionScheme::Absolute);

	this->setOffsets(x, Side::Left);
	this->setOffsets(y, Side::Top);

	this->doJavaScript("init_widget_token('" + this->id() + "', '" + filename + "');");

	// signal binding
	signal_move.connect(this, &widget_token::signal_move_callback);
}

void widget_token::on_close_click()
{
	on_close_event.emit();
	close();
}

void widget_token::on_shared_click()
{
	change_shared(! shared);
}

void widget_token::animate_position(int top, int left)
{
	this->addStyleClass("widget_token_animated", true);
	this->setOffsets(top, Side::Top);
	this->setOffsets(left, Side::Left);
}

void widget_token::signal_move_callback(int top, int left)
{
	on_move_event.emit(top, left);
}

void widget_token::close()
{
	this->parent()->removeWidget(this);
}

void widget_token::change_shared(bool shared)
{
	this->shared = shared;
	/* if (shared) */
	/* { */
	/* 	button_shared->setStyleClass("widget_image_shared widget_image_shared_yes"); */
	/* } */
	/* else */
	/* { */
	/* 	button_shared->setStyleClass("widget_image_shared widget_image_shared_no"); */
	/* } */
	on_shared_event.emit(shared);
}

void widget_token::change_token_visibility(bool visible)
{
	this->visible = visible;
	if (visible)
	{
		removeStyleClass("visibility_hidden", true);
	}
	else
	{
		addStyleClass("visibility_hidden", true);
	}
}

/****    widget_drag_token    ****/

widget_drag_token::widget_drag_token() : widget_drag_token("")
{
}

widget_drag_token::widget_drag_token(string filename) : wcontainer()
{
	setStyleClass("widget_drag_token");
	this->filename = filename;
	decorationStyle().setBackgroundImage(filename);
}

