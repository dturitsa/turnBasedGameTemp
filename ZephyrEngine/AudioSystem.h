#pragma once
#include "AudioEngine.h"
#include "System.h"

#define DEFAULT_VOLUME 5
#define GAMEPLAY_VOLUME 5
#define SOUNDTRACK_MENU "Assets/Music/MenuSong.mp3"
#define SOUNDTRACK_GAMEPLAY "Assets/Music/QuarrellingSeas.mp3"

class AudioSystem : public System
{
public:
	AudioEngine Audio;
	string currentBGM;
	int loadedLevel;
	int lastLevel = 2;
	int audioMute;

	AudioSystem(MessageBus* mbus);
	~AudioSystem();

	void startSystemLoop();
	void handleMessage(Msg *msg);

	void PlayMusic(string songName, float volume);
	void StopMusic(string songName);
	void PlaySfx(string soundName, float x, float y, float volume);
	void StopSfx(string soundName);
	void MuteAudio(bool mute);
	void StopAllAudio();

	int timeFrame = 100;
};

