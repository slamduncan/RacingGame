#include <stdio.h>
#include "Renderer.h"
#include "Entity.h"


// Other init
// ie. Physics, AI, Renderer, Container for ents?
Renderer ren;

/*
*	Handles what to do when key has been pressed
*
*/
void handle_key_down( SDL_keysym* keysym )
{
    switch( keysym->sym ) 
	{
		case SDLK_ESCAPE:
			ren.quitSDL();
			break;
		default:
			break;
    }

}

/*
*	Handles SDL events that are sent by user
*/
void process_events()
{
    /* Our SDL event placeholder. */
    SDL_Event event;

    /* Grab all the events off the queue. */
    while(SDL_PollEvent( &event )) {

        switch( event.type ) {
        case SDL_KEYDOWN:
            /* Handle key presses. */
            handle_key_down( &event.key.keysym );
            break;
        case SDL_QUIT:
            /* Handle quit requests (like Ctrl-c). */
            // QUIT SDL
			ren.quitSDL();
            break;
        }

    }

}

// Engine Main
int main(int argc, char** argv)
{
	// INITIALIZATIONS
	ren.initSDL();	// init SDL for drawing window
	ren.initGL(1280, 720);	// initializing opengl stuff

	// game loop
	while(1)
	{
		// Phyiscs


		// Inputs
		process_events();

		// AI



		// Compute FPS
		// Render
		// draw code goes here
		ren.clearGL();	// clear the screen
		ren.draw();		// draw things to the buffer
		ren.updateGL();	// update the screen

		// Misc?
	}

	return 0;
}