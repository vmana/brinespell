#ifndef widget_home_H
#define widget_home_H

#include "wcontainer.h"
#include "widget_search.h"
#include "widget_audio.h"
#include "widget_dice.h"
#include "widget_chat.h"
#include "widget_image.h"
#include "token/widget_token.h"
#include "character/widget_character.h"
#include "party/widget_party.h"
#include "db/campaign.h"

class widget_home : public wcontainer
{
	public:
		shared_ptr<campaign> p_campaign;
		widget_search *search;
		widget_audio *audio;
		widget_dice *dices;
		widget_chat *chat;
		widget_character *character;
		widget_party *party;
		WContainerWidget *dynamic_images;
		WContainerWidget *dynamic_tokens;

		widget_home();

		void global_key_pressed(WKeyEvent e);
		void search_master_open(string filename);
		void dropEvent(WDropEvent e);

		/****    static call from broadcast    ****/

		// audio
		static void change_audio_track(string filename);
		static void switch_pause_audio_track(bool paused);
		//dice
		static void throw_dice(string notation, string rand_init);
		// chat
		static void chat_message(string message);
		// images
		string open_image(string filename); // return image id
		static void open_shared_image(string filename, string id);
		static void move_image(string id, int top, int left);
		static void resize_image(string id, int top, int left, int width, int height);
		static void zoom_image(string id, int zoom_w, int zoom_h, int zoom_x, int zoom_y);
		static void switch_mode_image(string id, string mode);
		static void change_image_visibility(string id, bool visible);
		static void close_image(string id);
		static widget_image* search_image(string id);
		// token
		string open_token(string filename, int x, int y); // return token id
		static void open_shared_token(string filename, string id);
		static void move_token(string id, int top, int left);
		static void change_token_visibility(string id, bool visible);
		static void close_token(string id);
		static widget_token* search_token(string id);
		// ally
		static void update_ally_hp(int player_id, int percent, string helper);
};

#endif // widget_home_H

