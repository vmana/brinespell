#include "widget_audio.h"
#include "soma.h"

widget_audio::widget_audio() : wcontainer("audio")
{
	setStyleClass("widget_audio");
	load_audio("");
}

void widget_audio::render_widget()
{
	// remove everything, and create the widget
	// we need to do this since wt doesn't add source after the first rendering
	load("audio");

	// button play / pause
	button_play_pause = bindNew<WText>("button_play_pause");
	button_play_pause->setStyleClass("widget_audio_button widget_audio_button_play");
	button_play_pause->setToolTip("Pause / Play, Global HotKey : Space");

	// progress bar
	progress_bar = bindNew<WProgressBar>("progress_bar");
	progress_bar->setStyleClass("widget_audio_progress");

	// button volume
	button_volume = bindNew<WText>("button_volume");
	button_volume->setStyleClass("widget_audio_button widget_audio_button_volume");

	// volume bar
	volume_bar = bindNew<WProgressBar>("volume_bar");
	volume_bar->setStyleClass("widget_audio_volume");

	// mediaplayer
	mediaplayer = bindNew<WMediaPlayer>("mediaplayer", MediaType::Audio);
	mediaplayer->setControlsWidget(0);
	mediaplayer->setProgressBar(MediaPlayerProgressBarId::Time, progress_bar);
	mediaplayer->setProgressBar(MediaPlayerProgressBarId::Volume, volume_bar);
	mediaplayer->setVolume(current_volume);

	// signals binding
	mediaplayer->ended().connect([&](){ mediaplayer->play(); });
	/* mediaplayer->playbackStarted().connect([&](){ button_play_pause->setStyleClass("widget_audio_button widget_audio_button_pause"); }); */
	/* mediaplayer->playbackPaused().connect([&](){ button_play_pause->setStyleClass("widget_audio_button widget_audio_button_play"); }); */
	button_play_pause->clicked().connect(this, &widget_audio::on_play_pause_click);
	volume_bar->mouseWheel().connect(this, &widget_audio::on_volume_mouse_wheel);
	mediaplayer->volumeChanged().connect([&](const double &v){ current_volume = v; }); // keep track of volume when re-rendering

	string ext = file::extension(audio_filename);
	if (ext == "mp4") mediaplayer->addSource(MediaEncoding::M4A, audio_filename);
	else if (ext == "mp3") mediaplayer->addSource(MediaEncoding::MP3, audio_filename);
	else if (ext == "webm") mediaplayer->addSource(MediaEncoding::WEBMA, audio_filename);
	else if (ext == "wav") mediaplayer->addSource(MediaEncoding::WAV, audio_filename);
	else if (ext == "ogg") mediaplayer->addSource(MediaEncoding::OGA, audio_filename);
}

void widget_audio::play()
{
	mediaplayer->play();
	button_play_pause->setStyleClass("widget_audio_button widget_audio_button_pause");
}

void widget_audio::pause()
{
	mediaplayer->pause();
	button_play_pause->setStyleClass("widget_audio_button widget_audio_button_play");
}

void widget_audio::on_play_pause_click()
{
	if (mediaplayer->playing())
	{
		mediaplayer->pause();
		button_play_pause->setStyleClass("widget_audio_button widget_audio_button_play");
		on_switch_pause_event.emit(true);
	}
	else
	{
		mediaplayer->play();
		button_play_pause->setStyleClass("widget_audio_button widget_audio_button_pause");
		on_switch_pause_event.emit(false);
	}
}

void widget_audio::on_volume_mouse_wheel(const WMouseEvent &event)
{
	if (event.wheelDelta() > 0) // up
	{
		double new_volume = mediaplayer->volume() + 0.1;
		if (new_volume > 1.0) new_volume = 1.0;
		mediaplayer->setVolume(new_volume);
	}
	else if (event.wheelDelta() < 0) // down
	{
		double new_volume = mediaplayer->volume() - 0.1;
		if (new_volume < 0.0) new_volume = 0.0;
		mediaplayer->setVolume(new_volume);
	}
}

void widget_audio::load_audio(string filename)
{
	if (mediaplayer) mediaplayer->stop();
	audio_filename = filename;
	render_widget();
}
