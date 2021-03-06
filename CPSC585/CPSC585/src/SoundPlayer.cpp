#include "SoundPlayer.h"

SoundPlayer::SoundPlayer()
{
	ListenerPos[0] = 0.0;
	ListenerPos[1] = 0.0;
	ListenerPos[2] = 0.0;
}

void SoundPlayer::LoadSoundFile(ALbyte* FileName, ALuint& Source, ALuint& buffer, float SourcePos[3], ALboolean Looping)
{
	alDeleteSources(1, &Source);
	alDeleteBuffers(1, &buffer);

	alDopplerFactor(1.0);
	alDopplerVelocity(100.0);
	//alDistanceModel(AL_LINEAR_DISTANCE);

	ALfloat listenerPos[]={ListenerPos[0], ListenerPos[1], ListenerPos[2]};
	ALfloat listenerVel[]={0.0,0.0,0.0};
	ALfloat listenerOri[]={0.0,0.0,0.0, 0.0,0.0,0.0};

	ALfloat source0Pos[]={ SourcePos[0], SourcePos[1], SourcePos[2]};
	ALfloat source0Vel[]={ 0.0, 0.0, 0.0};

	//ALuint  buffer;

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
    	//printf("init() - No errors yet.");
	}

	ALboolean b;

	alutLoadWAVFile(FileName,&format,&data,&size,&freq,&b);
    alBufferData(buffer,format,data,size,freq);
    alutUnloadWAV(format,data,size,freq);

	alGetError(); /* clear error */
	alGenSources(1, &Source);

	if(alGetError() != AL_NO_ERROR) 
	{
    	printf("- Error creating sources !!\n");
	}
	else
	{
    	//printf("init - no errors after alGenSources\n");
	}

	alSourcef(Source, AL_PITCH, 1.0f);
	alSourcef(Source, AL_GAIN, 0.3f);
	alSourcefv(Source, AL_POSITION, source0Pos);
	alSourcefv(Source, AL_VELOCITY, source0Vel);
	alSourcei(Source, AL_BUFFER,buffer);
	alSourcei(Source, AL_LOOPING, Looping);
	alSourcef(Source, AL_REFERENCE_DISTANCE, 50.0);
	alSourcef(Source, AL_MAX_DISTANCE, 1000.0);
	//alSourcef(Source, AL_ROLLOFF_FACTOR, 1.0);

	alSourcePlay(Source);
}

void SoundPlayer::UpdateListenerPosition(float ListenerPosition[3])
{
	ListenerPos[0] = ListenerPosition[0];
	ListenerPos[1] = ListenerPosition[1];
	ListenerPos[2] = ListenerPosition[2];
}