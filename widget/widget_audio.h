#ifndef widget_audio_H
#define widget_audio_H

#include "wcontainer.h"
#include <Wt/WMediaPlayer.h>
#include <Wt/WProgressBar.h>

class widget_audio : public wcontainer
{
	protected:
		string audio_filename;
		double current_volume = 0.5;

	public:
		WMediaPlayer *mediaplayer = NULL;
		WText *button_play_pause;
		WProgressBar *progress_bar;
		WText *button_volume;
		WProgressBar *volume_bar;

		Signal<bool> on_switch_pause_event;

		widget_audio();
		void play();
		void pause();
		void on_play_pause_click();
		void on_volume_mouse_wheel(const WMouseEvent &event);
		void render_widget();
		void load_audio(string filename);
};

#endif // widget_audio_H

