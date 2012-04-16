#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include "al.h"
#include "alc.h"
#include "alut.h"

#include <stdio.h>

class SoundPlayer
{
public:
	SoundPlayer();
	void LoadSoundFile(ALbyte* FileName, ALuint& Source, ALuint& buffer, float SourcePos[3], ALboolean Looping  = AL_FALSE);
	void UpdateListenerPosition(float ListenerPosition[3]);

private:
	float ListenerPos[3];
};

#endif SOUNDPLAYER_H