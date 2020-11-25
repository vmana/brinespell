#ifndef widget_home_H
#define widget_home_H

#include "wcontainer.h"
#include "widget_dynamic.h"
#include "widget_search.h"
#include "widget_audio.h"
#include "widget_dice.h"
#include "widget_chat.h"
#include "character/widget_character.h"
#include "party/widget_party.h"
#include "db/campaign.h"

class widget_home : public wcontainer
{
	protected:
		string shell_load_search =
			"cd /dalaran/brinespell/data && find . -name '*' -type f | grep -v '/campaign/' | grep -v '.token.png' | sed -e 's,^\\./,,'";
	public:
		shared_ptr<campaign> p_campaign;
		widget_dynamic *dynamic = NULL;
		widget_search *search;
		widget_audio *audio;
		widget_dice *dices;
		widget_chat *chat;
		widget_character *character;
		widget_party *party;
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
		// ally
		static void update_ally_hp(int player_id, int percent, string helper);
};

#endif // widget_home_H

