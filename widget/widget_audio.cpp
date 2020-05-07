#include "widget_audio.h"
#include "soma.h"

widget_audio::widget_audio() : wcontainer("audio")
{
	setStyleClass("widget_audio");
	audio_filename = "data/a.mp4";
	render_widget();
}

void widget_audio::render_widget()
{
	// remove everything, and create the widget
	// we need to do this since wt doesn't add source after the first rendering

	load("audio");

	// button play / pause
	button_play_pause = bindNew<WText>("button_play_pause");
	button_play_pause->setStyleClass("widget_audio_button widget_audio_button_play");

	// progress bar
	progress_bar = bindNew<WProgressBar>("progress_bar");
	progress_bar->setStyleClass("widget_audio_progress");

	// button volume
	button_volume = bindNew<WText>("button_volume");
	button_volume->setStyleClass("widget_audio_button widget_audio_button_volume");

	// volume bar
	volume_bar = bindNew<WProgressBar>("volume_bar");
	volume_bar->setStyleClass("widget_audio_volume");

	// player
	player = bindNew<WMediaPlayer>("player", MediaType::Audio);
	player->setControlsWidget(0);
	player->setProgressBar(MediaPlayerProgressBarId::Time, progress_bar);
	player->setProgressBar(MediaPlayerProgressBarId::Volume, volume_bar);
	player->setVolume(current_volume);

	// signals binding
	player->ended().connect([&](){ player->play(); });
	player->playbackStarted().connect([&](){ button_play_pause->setStyleClass("widget_audio_button widget_audio_button_pause"); });
	player->playbackPaused().connect([&](){ button_play_pause->setStyleClass("widget_audio_button widget_audio_button_play"); });
	button_play_pause->clicked().connect(this, &widget_audio::on_play_pause_click);
	button_volume->clicked().connect([&](){ this->load_audio("data/rest.webm"); });
	player->volumeChanged().connect([&](const double &v){ current_volume = v; }); // keep track of volume when re-rendering

	player->addSource(MediaEncoding::M4A, audio_filename);
	string ext = file::extension(audio_filename);
	if (ext == "mp4") player->addSource(MediaEncoding::M4A, audio_filename);
	else if (ext == "mp3") player->addSource(MediaEncoding::MP3, audio_filename);
	else if (ext == "webm") player->addSource(MediaEncoding::WEBMA, audio_filename);
	else if (ext == "wav") player->addSource(MediaEncoding::WAV, audio_filename);
	else if (ext == "ogg") player->addSource(MediaEncoding::OGA, audio_filename);
}

void widget_audio::on_play_pause_click()
{
	if (player->playing())
	{
		player->pause();
		button_play_pause->setStyleClass("widget_audio_button widget_audio_button_play");
	}
	else
	{
		player->play();
		button_play_pause->setStyleClass("widget_audio_button widget_audio_button_pause");
	}
}

void widget_audio::load_audio(string filename)
{
	player->stop();
	audio_filename = filename;
	render_widget();
}
