#ifndef widget_home_H
#define widget_home_H

#include "wcontainer.h"
#include "widget_search.h"
#include "widget_audio.h"
#include "db/campaign.h"

class widget_home : public wcontainer
{
	public:
		shared_ptr<campaign> p_campaign;
		widget_search *search;
		widget_audio *audio;

		widget_home();
		// static call from broadcast
		static void change_audio_track(string filename);
};

#endif // widget_home_H

