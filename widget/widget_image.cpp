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
	signal_resize(this, "signal_resize"),
	signal_zoom(this, "signal_zoom")
{
	setId(id);
	this->share = share;

	int init_top = 200;
	int init_left = 600;

	setStyleClass("widget_image");
	setPositionScheme(PositionScheme::Absolute);
	button_close = bindNew<WText>("button_close");
	button_close->setStyleClass("widget_image_close");
	button_view_mode = bindNew<WText>("button_view_mode");
	button_view_mode->setStyleClass("widget_image_view_mode widget_image_view_contain");

	this->setOffsets(init_top, Side::Top);
	this->setOffsets(init_left, Side::Left);

	this->doJavaScript("init_widget_image('" + this->id() + "', '" + filename + "');");

	// signal binding
	button_close->clicked().connect(this, &widget_image::on_close_click);
	button_view_mode->clicked().connect(this, &widget_image::on_view_mode_click);
	signal_move.connect(this, &widget_image::signal_move_callback);
	signal_resize.connect(this, &widget_image::signal_resize_callback);
	signal_zoom.connect(this, &widget_image::signal_zoom_callback);
}

void widget_image::on_close_click()
{
	if (share) on_close_event.emit();
	close();
}

void widget_image::on_view_mode_click()
{
	if (current_mode == "zoom")
	{
		change_view_mode("contain");
	}
	else
	{
		change_view_mode("zoom");
	}

	if (share) on_view_mode_event.emit(current_mode);
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

void widget_image::animate_zoom(int zoom_w, int zoom_h, int zoom_x, int zoom_y)
{
	this->addStyleClass("widget_image_animated", true);

	string zoom_values =
		convert::int_string(zoom_w) + ";"
		+ convert::int_string(zoom_h) + ";"
		+ convert::int_string(zoom_x) + ";"
		+ convert::int_string(zoom_y);

	this->doJavaScript("wt_force_zoom('" + this->id() + "', '" +zoom_values + "');");
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

void widget_image::signal_zoom_callback(int zoom_w, int zoom_h, int zoom_x, int zoom_y)
{
	if (!share) return;
	on_zoom_event.emit({zoom_w, zoom_h, zoom_x, zoom_y});
}

void widget_image::close()
{
	S->main_div->removeWidget(this);
}

void widget_image::change_view_mode(string mode)
{
	if (mode == "zoom")
	{
		current_mode = "zoom";
		button_view_mode->setStyleClass("widget_image_view_mode widget_image_view_zoom");

	}
	else
	{
		current_mode = "contain";
		button_view_mode->setStyleClass("widget_image_view_mode widget_image_view_contain");
	}
	/* this->doJavaScript("w_image_switch_view('" + this->id() + "', '" + current_mode + "');"); */
}
