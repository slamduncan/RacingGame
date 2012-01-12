#include <iostream>
#include <fstream>
#include <SDL.h>
//#pragma comment(lib,"SDL.lib") 
//#pragma comment(lib,"SDLmain.lib") 
//#include "C:\Program Files\CPSC585\SDL\SDL-1.2.14\include\SDL_joystick.h"
using namespace std;

int mainTest(int argc, char *argv[])
{
if (SDL_Init(SDL_INIT_JOYSTICK|SDL_INIT_VIDEO) < 0 )
{
	fprintf(stderr, "Couldn't inititalize SDL: %s\n", SDL_GetError());
	exit(1);
}
SDL_JoystickEventState(SDL_ENABLE);
SDL_Event cntrlEvent;
SDL_Joystick *stick = NULL;
int num_controller;
num_controller = SDL_NumJoysticks();

//Example of finding the # of connected controllers
if (num_controller >0){
	cout << "Found Controllers\n";
	for (int i = 0; i < num_controller; i++){
			cout << SDL_JoystickName(i) << "\n";
	}
}

if (num_controller > 1){
	cout << "Found stuff\n";
		stick = SDL_JoystickOpen(0); //Opens the joystick to allow it to get events.
		if (stick == NULL){
			cout << "Issue with joystick"; return 0; //Issue with the joystick.
		}
}
//
//XBOX 360 Controler Maps
//
//Hat - dpad
//
//buttons:
//0 A
//1 B
//2 X
//3 Y
//4 L bumper
//5 R bumper
//6 Back
//7 Start
//8 L Analogue Click
//9 R Analogue Click
//
//Axis 
//0 L Analogue - LEFT : RIGHT
//1 L Analogue - TOP : BOTTOM
//2 L Trigger - positive, R Trigger - minus
//3 R Analogue - TOP : BOTTOM
//4 R Analogue - LEFT : RIGHT



bool running = true;
while(running){	
	while(SDL_PollEvent(&cntrlEvent)){
		cout << "Found event";			
		if(cntrlEvent.type == SDL_JOYAXISMOTION)
		{
			//Note can check here which stick it is with event.jaxis.which == stick index
			//x-axis moved (x-axis is 0)
			if(cntrlEvent.jaxis.axis == 0)
			{
				//NOTE: range for value is from -32768 to 32767 (so anything btw -8000 and 8000 is usually useless)
				cout << cntrlEvent.jaxis.value << "x-axis\n";
			}
			//y-axis moved (y-axis is 1)
			else if (cntrlEvent.jaxis.axis == 1)
			{
				cout << cntrlEvent.jaxis.value << " y-axis\n";
			}
			//Trigger Buttons (Note they are treated as one axis, left is +, right is -)
			else if(cntrlEvent.jaxis.axis == 2)
			{
				cout << cntrlEvent.jaxis.value << "Trigger value \n";
			}
		}
		if (cntrlEvent.type == SDL_JOYBUTTONDOWN){
			//X-button was pressed
			if (cntrlEvent.jbutton.button == 2){
				//running = false;
				cout << "X Pressed";
			}
		}
	}
}
	SDL_JoystickClose(stick); // Don't forget to close the joystick
	SDL_Quit();
}