#ifndef widget_dynamic_H
#define widget_dynamic_H

#include "wcontainer.h"
#include "widget_image.h"
#include "token/widget_token.h"
#include "token/wtoken_player.h"
#include "token/wtoken_npc.h"

class widget_dynamic : public wcontainer
{
	public:
		WContainerWidget *images;
		WContainerWidget *tokens;

		widget_dynamic();
		static WContainerWidget* instance_images();
		static WContainerWidget* instance_tokens();
		void load_session_dynamics(); // load dynamic data on session join
		static void send_session_dynamics(string session_id);

		/****    images    ****/
		widget_image* open_image(dbo::ptr<player> creator, string filename);
		static void open_shared_image(long long int creator_id, string filename, string id, int top, int left, bool visible);
		static void move_image(string id, int top, int left);
		static void resize_image(string id, int top, int left, int width, int height);
		static void zoom_image(string id, int zoom_w, int zoom_h, int zoom_x, int zoom_y);
		static void switch_mode_image(string id, string mode);
		static void change_image_visibility(string id, bool visible);
		static void close_image(string id);
		static widget_image* search_image(string id);

		/****    tokens    ****/
		wtoken_player* open_token_player(dbo::ptr<player> p_player, int top, int left);
		wtoken_npc* open_token_npc(shared_ptr<npc> p_npc, int top, int left);
		static void open_shared_token(string filename, string id);
		static void open_shared_token_player(long long int player_id, string id, int top, int left);
		static void open_shared_token_npc(shared_ptr<npc> p_npc, string id, int top, int left);
		static void move_token(string id, int top, int left);
		static void close_token(string id);
		static void add_token_initiative(shared_ptr<npc> p_npc);
		static void remove_token_initiative(shared_ptr<npc> p_npc);
		static void update_tokens_initiative();
		static widget_token* search_token(string id);
		static wtoken_player* search_token_player(dbo::ptr<player> p_player);
		static wtoken_npc* search_token_npc(string id);
};

#endif // widget_dynamic_H

