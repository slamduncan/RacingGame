#ifndef TIMER_H
#define TIMER_H

#include "SDL.h"

class Timer
{
private:
	int startTicks;
	int pausedTicks;
	
	bool started;
	bool paused;
	

public:
	Timer();
	~Timer();
	
	void start();
	void stop();
	void pause();
	void unpause();
	
	int get_ticks();
	
	bool isStarted();
	bool isPaused();
};

#endif