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

void widget_dynamic::load_session_dynamics()
{
	broadcast::others(&widget_dynamic::send_session_dynamics, S->sessionId());
}

void widget_dynamic::send_session_dynamics(string session_id)
{
	auto p_images = widget_dynamic::instance_images();
	auto p_tokens = widget_dynamic::instance_tokens();
	if (!p_images || !p_tokens) return;
	WServer *wt_server = WServer::instance();
	if (!wt_server) return;

	// send images
	for (auto &child : p_images->children())
	{
		auto p_image = dynamic_cast<widget_image*>(child);
		if (!p_image) continue; // should never happen

		wt_server->post(session_id, bind
		(
			&widget_dynamic::open_shared_image,
			p_image->creator.id(),
			p_image->filename,
			p_image->id(),
			(int)p_image->offset(Side::Top).value(),
			(int)p_image->offset(Side::Left).value(),
			p_image->is_visible()
		));
	}

	// send tokens
	for (auto &child : p_tokens->children())
	{
		// test for wtoken_player
		auto p_token = dynamic_cast<wtoken_player*>(child);
		if (p_token)
		{
			wt_server->post(session_id, bind
			(
				&widget_dynamic::open_shared_token_player,
				p_token->p_player.id(),
				p_token->id(),
				(int)p_token->offset(Side::Top).value(),
				(int)p_token->offset(Side::Left).value()
			));
		}
	}
}

/****    image    ****/

widget_image* widget_dynamic::open_image(dbo::ptr<player> creator, string filename)
{
	widget_image *img = NULL;
	string id = mana::randstring(16);
	img = images->addNew<widget_image>(creator, filename, id);

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

	return img;
}

void widget_dynamic::open_shared_image(long long int creator_id, string filename, string id, int top, int left, bool visible)
{
	auto p_images = widget_dynamic::instance_images();
	if (!p_images) return;

	// load p_player
	dbo_session session;
	auto creator = session->load<player>(creator_id);

	auto img = search_image(id);
	if (img) return; // already exists, don't create

	img = p_images->addNew<widget_image>(creator, filename, id, top, left, visible);

	// check if I'm the original creator to get full control
	// happens if I disconnect, and reconnect while someone still has the image opened

	auto p_soma = soma::application();
	if (creator.id() != p_soma->p_shadow.id()) return; // not the original creator

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
	if (img)
	{
		img->change_image_visibility(visible);
		img->change_shared(true);
	}
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

widget_token* widget_dynamic::open_token(string filename, int top, int left)
{
	widget_token *token = NULL;
	string id = mana::randstring(16);
	token = tokens->addNew<widget_token>(filename, id, top, left);

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

	return token;
}

wtoken_player* widget_dynamic::open_token_player(dbo::ptr<player> p_player, int top, int left)
{
	// search if a token for this player already exists
	// if if exists, only move the token
	wtoken_player *token = NULL;
	token = search_token_player(p_player);
	if (token)
	{
		// just return if it didn't move
		if (token->offset(Side::Top) == top && token->offset(Side::Left) == left) return token;

		// broadcast move to other players
		broadcast::all(&widget_dynamic::move_token, token->id(), top, left);
		return token;
	}

	string id = mana::randstring(16);
	token = tokens->addNew<wtoken_player>(p_player, id, top, left);

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

	broadcast::others(&widget_dynamic::open_shared_token_player, p_player.id(), id, top, left);

	return token;
}

void widget_dynamic::open_shared_token(string filename, string id)
{
	auto p_tokens = widget_dynamic::instance_tokens();
	if (!p_tokens) return;
	p_tokens->addNew<widget_token>(filename, id, false);
}

void widget_dynamic::open_shared_token_player(long long int player_id, string id, int top, int left)
{
	auto p_tokens = widget_dynamic::instance_tokens();
	if (!p_tokens) return;

	// load p_player
	dbo_session session;
	auto p_player = session->load<player>(player_id);

	// search if a token for this player already exists
	// if if exists, only move the token
	auto token = search_token_player(p_player);
	if (token)
	{
		// just return if it didn't move
		if (token->offset(Side::Top) == top && token->offset(Side::Left) == left) return;
		move_token(id, top, left);
		return;
	}

	token = p_tokens->addNew<wtoken_player>(p_player, id, top, left);

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

wtoken_player* widget_dynamic::search_token_player(dbo::ptr<player> p_player)
{
	wtoken_player *ret = NULL;
	auto p_tokens = widget_dynamic::instance_tokens();
	if (!p_tokens) return ret;

	// search for a child with this id
	for (auto &child : p_tokens->children())
	{
		auto p_child = dynamic_cast<wtoken_player*>(child);
		if (! p_child) continue; // don't care about this token
		if (p_child->p_player.id() == p_player.id())
			return p_child;
	}
	return ret;
}
