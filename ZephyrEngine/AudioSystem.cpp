#include "AudioSystem.h"

AudioSystem::AudioSystem(MessageBus* mbus) : System(mbus)
{
}

AudioSystem::~AudioSystem()
{
}

void AudioSystem::startSystemLoop()
{
	clock_t thisTime = clock();

	int currentGameTime = 0;
	while (alive) {
		thisTime = clock();
		if (thisTime  < currentGameTime) {
			Sleep(currentGameTime - thisTime);
		}

		currentGameTime = thisTime + timeFrame;
		handleMsgQ();
	}
}

void AudioSystem::handleMessage(Msg *msg)
{
	System::handleMessage(msg);

	switch (msg->type) {
	case LEVEL_LOADED:
		loadedLevel = atoi(msg->data.c_str());
		if (loadedLevel == 2)
		{
			if (loadedLevel != lastLevel)
			{
				PlayMusic(SOUNDTRACK_GAMEPLAY, GAMEPLAY_VOLUME);
				StopMusic(SOUNDTRACK_MENU);
			}
		}
		else
		{
			if (lastLevel == 2)
			{
				PlayMusic(SOUNDTRACK_MENU, DEFAULT_VOLUME);
				StopMusic(SOUNDTRACK_GAMEPLAY);
			}
		}
			
		break;
	case AUDIO_MUTE:
		audioMute = atoi(msg->data.c_str());
		if(audioMute == 1)
			MuteAudio(true);
		else
			MuteAudio(false);
		break;
	default:
		break;
	}
	lastLevel = loadedLevel;
}

void AudioSystem::PlayMusic(string songName, float volume)
{
	Audio.LoadSound(songName, true, true, false);
	Audio.PlayAudio(songName, Vector3{ 0, 0, 0 }, volume);
}

void AudioSystem::StopMusic(string songName)
{
	Audio.UnLoadSound(songName);
}

void AudioSystem::PlaySfx(string soundName, float x, float y, float volume)
{
	Audio.LoadSound(soundName, true, false, false);
	Audio.PlayAudio(soundName, Vector3{ x, y, 0 }, volume);
}

void AudioSystem::StopSfx(string soundName)
{
	Audio.UnLoadSound(soundName);
}

void AudioSystem::MuteAudio(bool mute)
{
	Audio.Mute(mute);
}

void AudioSystem::StopAllAudio()
{
	Audio.StopAll();
}