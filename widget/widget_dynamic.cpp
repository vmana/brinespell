#include "widget_dynamic.h"
#include "widget_home.h"
#include "soma.h"

widget_dynamic::widget_dynamic() : wcontainer("dynamic")
{
	/* setStyleClass("widget_dynamic"); */
	images = bindNew<WContainerWidget>("images");
	tokens = bindNew<WContainerWidget>("tokens");
}

WContainerWidget* widget_dynamic::instance_images()
{
	auto p_soma = soma::application();
	if (!p_soma->view_home) return NULL;
	return p_soma->view_home->dynamic->images;
}

WContainerWidget* widget_dynamic::instance_tokens()
{
	auto p_soma = soma::application();
	if (!p_soma->view_home) return NULL;
	return p_soma->view_home->dynamic->tokens;
}

/****    image    ****/

string widget_dynamic::open_image(string filename)
{
	string id = mana::randstring(16);
	auto img = images->addNew<widget_image>(filename, id);

	// signals binding
	img->on_move_event.connect([=](int top, int left)
	{
		broadcast::others(&widget_dynamic::move_image, id, top, left);
	});
	// needs tuple since wt signal connect only allows 3 parameters max
	img->on_resize_event.connect([=](tuple<int, int, int, int> pos)
	{
		auto &[top, left, width, height] = pos;
		broadcast::others(&widget_dynamic::resize_image, id, top, left, width, height);
	});
	img->on_zoom_event.connect([=](tuple<int, int, int, int> pos)
	{
		auto &[zoom_w, zoom_h, zoom_x, zoom_y] = pos;
		broadcast::others(&widget_dynamic::zoom_image, id, zoom_w, zoom_h, zoom_x, zoom_y);
	});
	img->on_close_event.connect([=]()
	{
		broadcast::others(&widget_dynamic::close_image, id);
	});
	img->on_view_mode_event.connect([=](string mode)
	{
		broadcast::others(&widget_dynamic::switch_mode_image, id, mode);
	});
	img->on_shared_event.connect([=](bool shared)
	{
		broadcast::others(&widget_dynamic::change_image_visibility, id, shared);
	});

	return id;
}

void widget_dynamic::open_shared_image(string filename, string id)
{
	auto p_images = widget_dynamic::instance_images();
	if (!p_images) return;
	p_images->addNew<widget_image>(filename, id, false);
}

void widget_dynamic::move_image(string id, int top, int left)
{
	auto img = search_image(id);
	if (img) img->animate_position(top, left);
}

void widget_dynamic::resize_image(string id, int top, int left, int width, int height)
{
	auto img = search_image(id);
	if (img) img->animate_resize(top, left, width, height);
}

void widget_dynamic::zoom_image(string id, int zoom_w, int zoom_h, int zoom_x, int zoom_y)
{
	auto img = search_image(id);
	if (img) img->animate_zoom(zoom_w, zoom_h, zoom_x, zoom_y);
}

void widget_dynamic::switch_mode_image(string id, string mode)
{
	auto img = search_image(id);
	if (img) img->change_view_mode(mode);
}

void widget_dynamic::change_image_visibility(string id, bool visible)
{
	auto img = search_image(id);
	if (img) img->change_image_visibility(visible);
}

void widget_dynamic::close_image(string id)
{
	auto img = search_image(id);
	if (img) img->close();
}

widget_image* widget_dynamic::search_image(string id)
{
	widget_image *ret = NULL;
	auto p_images = widget_dynamic::instance_images();
	if (!p_images) return ret;

	// search for a child with this id
	for (auto &child : p_images->children())
	{
		if (child->id() == id)
			return (widget_image*)child;
	}
	return ret;
}

/****    token    ****/

string widget_dynamic::open_token(string filename, int x, int y)
{
	string id = mana::randstring(16);
	auto token = tokens->addNew<widget_token>(filename, id, x, y);

	// signals binding
	token->on_move_event.connect([=](int top, int left)
	{
		broadcast::others(&widget_dynamic::move_token, id, top, left);
	});
	token->on_close_event.connect([=]()
	{
		broadcast::others(&widget_dynamic::close_token, id);
	});
	token->on_shared_event.connect([=](bool shared)
	{
		broadcast::others(&widget_dynamic::change_token_visibility, id, shared);
	});

	return id;
}

void widget_dynamic::open_shared_token(string filename, string id)
{
	auto p_tokens = widget_dynamic::instance_tokens();
	if (!p_tokens) return;
	p_tokens->addNew<widget_token>(filename, id, false);
}

void widget_dynamic::move_token(string id, int top, int left)
{
	auto token = search_token(id);
	if (token) token->animate_position(top, left);
}

void widget_dynamic::change_token_visibility(string id, bool visible)
{
	auto token = search_token(id);
	if (token) token->change_token_visibility(visible);
}

void widget_dynamic::close_token(string id)
{
	auto token = search_token(id);
	if (token) token->close();
}

widget_token* widget_dynamic::search_token(string id)
{
	widget_token *ret = NULL;
	auto p_tokens = widget_dynamic::instance_tokens();
	if (!p_tokens) return ret;

	// search for a child with this id
	for (auto &child : p_tokens->children())
	{
		if (child->id() == id)
			return (widget_token*)child;
	}
	return ret;
}
