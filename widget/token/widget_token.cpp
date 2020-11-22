#include "widget_token.h"
#include "soma.h"

widget_token::widget_token(string filename) :
	widget_token(filename, mana::randstring(16))
{
}

widget_token::widget_token(string filename, string id) :
	widget_token(filename, id, 220, 550)
{
}

widget_token::widget_token(string filename, string id, int top, int left) :
	wcontainer("token/player"),
	signal_move(this, "signal_move"),
	signal_right_click(this, "signal_right_click")
{
	setStyleClass("widget_token");

	setId(id);
	setPositionScheme(PositionScheme::Absolute);

	this->setOffsets(top, Side::Top);
	this->setOffsets(left, Side::Left);

	tooltip = bindNew<widget_token_tooltip>("tooltip");

	this->doJavaScript("init_widget_token('" + this->id() + "', '" + filename + "');");

	// signal binding
	signal_move.connect(this, &widget_token::signal_move_callback);
	signal_right_click.connect(this, &widget_token::on_token_click);
	doubleClicked().connect(this, &widget_token::on_token_click);
}

void widget_token::on_token_click()
{
	tooltip->switch_visibility();
}

void widget_token::on_close_click()
{
	on_close_event.emit();
	close_token();
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

void widget_token::close_token()
{
	this->parent()->removeWidget(this);
}

/****    widget_token_tooltip    ****/

widget_token_tooltip::widget_token_tooltip() : wcontainer("token/tooltip")
{
	setStyleClass("widget_token_tooltip");
	bar = bindNew<WContainerWidget>("bar");
	bar->setStyleClass("bar");
	panel = bindNew<wcontainer>("panel", "token/tooltip_panel");
	panel->setStyleClass("panel");
}

void widget_token_tooltip::show_panel()
{
	visible = true;
	bar->setStyleClass("bar bar_extended");
	panel->addStyleClass(panel_extended_class);
}
void widget_token_tooltip::hide_panel()
{
	visible = false;
	bar->setStyleClass("bar");
	panel->removeStyleClass(panel_extended_class);
}

void widget_token_tooltip::switch_visibility()
{
	change_visibility(!visible);
}

void widget_token_tooltip::change_visibility(bool visible)
{
	if (visible) show_panel();
	else hide_panel();
}

/****    widget_drag_token    ****/

widget_drag_token::widget_drag_token() : wcontainer()
{
	setStyleClass("widget_drag_token");
}

widget_drag_token::widget_drag_token(dbo::ptr<player> p_player) : widget_drag_token()
{
	decorationStyle().setBackgroundImage(p_player->token_image());
}

