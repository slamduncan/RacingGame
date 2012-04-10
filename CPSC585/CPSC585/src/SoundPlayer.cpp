#include "SoundPlayer.h"

SoundPlayer::SoundPlayer()
{

}

void SoundPlayer::LoadBackgroundSoundFile(ALbyte* FileName)
{
	ALuint BackGroundSource = 1;

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
	alGenSources(1, &BackGroundSource);

	if(alGetError() != AL_NO_ERROR) 
	{
    	printf("- Error creating sources !!\n");
	}
	else
	{
    	printf("init - no errors after alGenSources\n");
	}

	alSourcef(BackGroundSource, AL_PITCH, 1.0f);
	alSourcef(BackGroundSource, AL_GAIN, 0.5f);
	alSourcefv(BackGroundSource, AL_POSITION, source0Pos);
	alSourcefv(BackGroundSource, AL_VELOCITY, source0Vel);
	alSourcei(BackGroundSource, AL_BUFFER,buffer);
	alSourcei(BackGroundSource, AL_LOOPING, AL_TRUE);

	alSourcePlay(BackGroundSource);
}

void SoundPlayer::LoadSoundFile(ALbyte* FileName, ALuint* Source, ALboolean Looping)
{
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
	alGenSources(1, Source);

	if(alGetError() != AL_NO_ERROR) 
	{
    	printf("- Error creating sources !!\n");
	}
	else
	{
    	printf("init - no errors after alGenSources\n");
	}

	alSourcef(*Source, AL_PITCH, 1.0f);
	alSourcef(*Source, AL_GAIN, 1.0f);
	alSourcefv(*Source, AL_POSITION, source0Pos);
	alSourcefv(*Source, AL_VELOCITY, source0Vel);
	alSourcei(*Source, AL_BUFFER,buffer);
	alSourcei(*Source, AL_LOOPING, Looping);

	alSourcePlay(*Source);
}