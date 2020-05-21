#ifndef widget_home_H
#define widget_home_H

#include "wcontainer.h"
#include "widget_search.h"
#include "widget_audio.h"
#include "widget_dice.h"
#include "widget_chat.h"
#include "widget_image.h"
#include "db/campaign.h"

class widget_home : public wcontainer
{
	public:
		shared_ptr<campaign> p_campaign;
		widget_search *search;
		widget_audio *audio;
		widget_dice *dices;
		widget_chat *chat;

		widget_image *tmp_img;

		widget_home();
		// static call from broadcast
		void search_master_open(string filename);
		static void change_audio_track(string filename);
		static void switch_pause_audio_track(bool paused);
		static void throw_dice(string notation, string rand_init);
		static void chat_message(string message);
		static string open_image(string filename); // return image id
		static void open_shared_image(string filename, string id);
		static void move_image(string id, int top, int left);
		static void resize_image(string id, int top, int left, int width, int height);
		static void switch_view_image(string id, string view);
		static void close_image(string id);
};

#endif // widget_home_H

