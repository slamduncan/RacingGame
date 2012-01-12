#include <iostream>
#include <fstream>
#include <SDL.h>
#include "InputController.h"
//#pragma comment(lib,"SDL.lib") 
//#pragma comment(lib,"SDLmain.lib") 
//#include "C:\Program Files\CPSC585\SDL\SDL-1.2.14\include\SDL_joystick.h"
using namespace std;

int main(int argc, char *argv[])
{
if (SDL_Init(SDL_INIT_JOYSTICK|SDL_INIT_VIDEO) < 0 )
{
	fprintf(stderr, "Couldn't inititalize SDL: %s\n", SDL_GetError());
	exit(1);
}
//SDL_JoystickEventState(SDL_ENABLE);
int num_controller;
num_controller = SDL_NumJoysticks();

//Example of finding the # of connected controllers
if (num_controller >0){
	cout << "Found Controllers\n";
	for (int i = 0; i < num_controller; i++){
			cout << SDL_JoystickName(i) << "\n";
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
InputController controls = InputController();
controls.initialize(0);
while(running){	
	if (controls.isXDown()){
		running = false;
	}
	else if (controls.isADown()) {
		cout << controls.getLeftAnalogStickX() << " X " << controls.getLeftAnalogStickY() << " Y \n";
	}
}
	SDL_Quit();
}