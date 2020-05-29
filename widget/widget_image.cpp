#include "widget_image.h"
#include "soma.h"

widget_image::widget_image()
	: widget_image("")
{
}

widget_image::widget_image(string filename) :
	widget_image(filename, mana::randstring(16))
{
}

widget_image::widget_image(string filename, string id) :
	wcontainer("image"),
	signal_move(this, "signal_move"),
	signal_resize(this, "signal_resize"),
	signal_view_mode(this, "signal_view_mode"),
	signal_zoom(this, "signal_zoom")
{
	setId(id);

	int init_top = 200;
	int init_left = 600;

	setStyleClass("widget_image");
	setPositionScheme(PositionScheme::Absolute);
	button_close = bindNew<WText>("button_close");
	button_close->setStyleClass("widget_image_close");

	button_shared = bindNew<WText>("button_shared");
	button_shared->setStyleClass("widget_image_shared widget_image_shared_yes");
	button_shared->setToolTip("Visible / Hidden for other players");

	this->setOffsets(init_top, Side::Top);
	this->setOffsets(init_left, Side::Left);

	this->doJavaScript("init_widget_image('" + this->id() + "', '" + filename + "');");

	// signal binding
	button_close->clicked().connect(this, &widget_image::on_close_click);
	button_shared->clicked().connect(this, &widget_image::on_shared_click);
	signal_move.connect(this, &widget_image::signal_move_callback);
	signal_resize.connect(this, &widget_image::signal_resize_callback);
	signal_view_mode.connect(this, &widget_image::signal_view_mode_callback);
	signal_zoom.connect(this, &widget_image::signal_zoom_callback);
}

void widget_image::on_close_click()
{
	on_close_event.emit();
	close();
}

void widget_image::on_shared_click()
{
	shared = !shared;
	if (shared)
	{
		button_shared->setStyleClass("widget_image_shared widget_image_shared_yes");
	}
	else
	{
		button_shared->setStyleClass("widget_image_shared widget_image_shared_no");
	}
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

	string size_values =
		convert::int_string(top) + ";"
		+ convert::int_string(left) + ";"
		+ convert::int_string(width) + ";"
		+ convert::int_string(height);


	// if in zoom mode, will trigger update_zoom_content
	this->doJavaScript("wt_image_border_resize('" + this->id() + "', '" + size_values + "');");
}

void widget_image::animate_zoom(int zoom_w, int zoom_h, int zoom_x, int zoom_y)
{
	this->addStyleClass("widget_image_animated", true);

	string zoom_values =
		convert::int_string(zoom_w) + ";"
		+ convert::int_string(zoom_h) + ";"
		+ convert::int_string(zoom_x) + ";"
		+ convert::int_string(zoom_y);

	this->doJavaScript("wt_image_force_zoom('" + this->id() + "', '" + zoom_values + "');");
}

void widget_image::signal_move_callback(int top, int left)
{
	on_move_event.emit(top, left);
}

void widget_image::signal_resize_callback(int top, int left, int width, int height)
{
	on_resize_event.emit({top, left, width, height});
}

void widget_image::signal_view_mode_callback(string mode)
{
	on_view_mode_event.emit(mode);
}

void widget_image::signal_zoom_callback(int zoom_w, int zoom_h, int zoom_x, int zoom_y)
{
	on_zoom_event.emit({zoom_w, zoom_h, zoom_x, zoom_y});
}

void widget_image::close()
{
	S->main_div->removeWidget(this);
}

void widget_image::change_view_mode(string mode)
{
	this->doJavaScript("wt_image_switch_view('" + this->id() + "', '" + mode + "');");
}
