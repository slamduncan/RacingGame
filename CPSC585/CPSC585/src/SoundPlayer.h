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
	void LoadBackgroundSoundFile(ALbyte* FileName);
	void LoadSoundFile(ALbyte* FileName, ALuint* Source, ALboolean Looping  = AL_FALSE);

private:
};

#endif SOUNDPLAYER_H