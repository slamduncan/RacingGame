#ifndef SOUND_H
#define SOUND_H

#include "al.h"
#include "alc.h"
#include "alut.h"

#include <stdio.h>

ALuint  source;

void LoadBackgroundSoundFile(ALbyte* FileName)
{
	ALCdevice* device = alcOpenDevice(NULL);
	ALCcontext* context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);

	ALfloat listenerPos[]={0.0,0.0,4.0};
	ALfloat listenerVel[]={0.0,0.0,0.0};
	ALfloat listenerOri[]={0.0,0.0,1.0, 0.0,1.0,0.0};

	ALfloat source0Pos[]={ -2.0, 0.0, 0.0};
	ALfloat source0Vel[]={ 0.0, 0.0, 0.0};

	ALuint  buffer;

	ALsizei size,freq;
	ALenum  format;
	ALvoid  *data;

	alListenerfv(AL_POSITION,listenerPos);
    alListenerfv(AL_VELOCITY,listenerVel);
    alListenerfv(AL_ORIENTATION,listenerOri);

	alGetError(); // clear any error messages

	// Generate buffers, or else no sound will happen!
	alGenBuffers(1, &buffer);

	if(alGetError() != AL_NO_ERROR) 
	{
    	printf("- Error creating buffers !!\n");
    	exit(1);
	}
	else
	{
    	printf("init() - No errors yet.");
	}

	ALboolean b;

	alutLoadWAVFile(FileName,&format,&data,&size,&freq,&b);
    alBufferData(buffer,format,data,size,freq);
    alutUnloadWAV(format,data,size,freq);

	alGetError(); /* clear error */
	alGenSources(1, &source);

	if(alGetError() != AL_NO_ERROR) 
	{
    	printf("- Error creating sources !!\n");
    	exit(2);
	}
	else
	{
    	printf("init - no errors after alGenSources\n");
	}

	alSourcef(source, AL_PITCH, 1.0f);
	alSourcef(source, AL_GAIN, 1.0f);
	alSourcefv(source, AL_POSITION, source0Pos);
	alSourcefv(source, AL_VELOCITY, source0Vel);
	alSourcei(source, AL_BUFFER,buffer);
	alSourcei(source, AL_LOOPING, AL_TRUE);

	alSourcePlay(source);
}


#endif SOUND_H