#include "widget_image.h"
#include "soma.h"

widget_image::widget_image()
	: widget_image("")
{
}

widget_image::widget_image(string filename, bool share) :
	widget_image(filename, mana::randstring(16), share)
{
}

widget_image::widget_image(string filename, string id, bool share) :
	wcontainer("image"),
	signal_move(this, "signal_move"),
	signal_resize(this, "signal_resize")
{
	setId(id);
	this->share = share;

	int init_top = 300;
	int init_left = 300;

	setStyleClass("widget_image");
	setPositionScheme(PositionScheme::Absolute);
	button_close = bindNew<WText>("button_close");
	button_close->setStyleClass("widget_image_close");
	button_switch_view = bindNew<WText>("button_switch_view");
	button_switch_view->setStyleClass("widget_image_switch_view widget_image_view_contain");

	this->setOffsets(init_top, Side::Top);
	this->setOffsets(init_left, Side::Left);

	this->doJavaScript("init_widget_image('" + this->id() + "', '" + filename + "');");

	// signal binding
	button_close->clicked().connect(this, &widget_image::on_close_click);
	button_switch_view->clicked().connect(this, &widget_image::on_switch_view_click);
	signal_move.connect(this, &widget_image::signal_move_callback);
	signal_resize.connect(this, &widget_image::signal_resize_callback);
}

void widget_image::on_close_click()
{
	if (share) on_close_event.emit();
	close();
}

void widget_image::on_switch_view_click()
{
	if (current_view == "cover")
	{
		switch_view("contain");
	}
	else
	{
		switch_view("cover");
	}

	if (share) on_switch_view_event.emit(current_view);
}

void widget_image::animate_position(int top, int left)
{
	this->addStyleClass("widget_image_animated", true);
	this->setOffsets(top, Side::Top);
	this->setOffsets(left, Side::Left);
}

void widget_image::animate_resize(int top, int left, int width, int height)
{
	this->addStyleClass("widget_image_animated", true);
	this->setOffsets(top, Side::Top);
	this->setOffsets(left, Side::Left);
	this->resize(width, height);
}

void widget_image::signal_move_callback(int top, int left)
{
	if (!share) return;
	on_move_event.emit(top, left);
}

void widget_image::signal_resize_callback(int top, int left, int width, int height)
{
	if (!share) return;
	on_resize_event.emit({top, left, width, height});
}

void widget_image::close()
{
	S->main_div->removeWidget(this);
}

void widget_image::switch_view(string view)
{
	if (view == "cover")
	{
		current_view = "cover";
		button_switch_view->setStyleClass("widget_image_switch_view widget_image_view_contain");
	}
	else
	{
		current_view = "contain";
		button_switch_view->setStyleClass("widget_image_switch_view widget_image_view_cover");
	}
	this->doJavaScript("w_image_switch_view('" + this->id() + "', '" + current_view + "');");
}
