#include "widget_image.h"
#include "soma.h"

widget_image::widget_image() : wcontainer("image")
{
	setStyleClass("widget_image");
	this->doJavaScript("init_widget_image('" + this->id() + "');");
}
