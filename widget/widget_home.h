#ifndef widget_home_H
#define widget_home_H

#include "wcontainer.h"
#include "widget_search.h"
#include "widget_audio.h"
#include "widget_dice.h"
#include "widget_chat.h"
#include "db/campaign.h"

class widget_home : public wcontainer
{
	public:
		shared_ptr<campaign> p_campaign;
		widget_search *search;
		widget_audio *audio;
		widget_dice *dices;
		widget_chat *chat;

		widget_home();
		// static call from broadcast
		static void change_audio_track(string filename);
		static void switch_pause_audio_track(bool paused);
		static void throw_dice(string notation, string rand_init);
		static void chat_message(string message);
};

#endif // widget_home_H

