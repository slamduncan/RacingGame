#include "Timer.h"

Timer::Timer()
{
	//Start the timer 
	started = true; 
	
	//Unpause the timer 
	paused = false; 
	
	//Get the current clock time 
	startTicks = SDL_GetTicks(); 
}

Timer::~Timer()
{

}


void Timer::start()
{
	started = true;
	paused = false;
	
	startTicks = SDL_GetTicks();
}
void Timer::stop()
{
	started = false;
	paused = false;
}
void Timer::pause()
{
	if( ( started == true ) && ( paused == false ) )
	{
		paused = true;

		pausedTicks = SDL_GetTicks() - startTicks;
	}
}
void Timer::unpause()
{
    //If the timer is paused
    if( paused == true )
    {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
	//If the timer is running
    if( started == true )
    {
        //If the timer is paused
        if( paused == true )
        {
            //Return the number of ticks when the timer was paused
            return pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }
    }

    //If the timer isn't running
    return 0;
}

bool Timer::isStarted()
{
	return started;
}
bool Timer::isPaused()
{
	return paused;
}