#include "InputController.h"

#include <iostream>
#include "EventSystemHandler.h"
#include "ButtonEvent.h"
#define MAX_AXIS_VAL 32767.0

InputController::InputController()
{
	A = B = X = Y = rightBump = leftBump = start = false;
	leftAnaX = leftAnaY = rightAnaX = rightAnaY = triggers = 0;
	normLeftAnaX = normLeftAnaY = normRightAnaX = normRightAnaY = normTriggers =  0.0;
}

void InputController::initSDLJoystick(){
	if (SDL_Init(SDL_INIT_JOYSTICK) < 0 ){
		fprintf(stderr, "Couldn't inititalize SDL Controller: %s\n", SDL_GetError());
		SDL_Quit();
		printf("Closing program...\n");
		exit(0);
	}
}
		

bool InputController::initialize(int controllerIndex){
	if (SDL_NumJoysticks() < controllerIndex+1){
		int temp = SDL_NumJoysticks();
		return false;
	}
	else{
		stick = NULL;
		InputControllerIdentifier = controllerIndex;	
		stick = SDL_JoystickOpen(controllerIndex);
		if(stick == NULL)
			return false;
	}
	 e = EventSystemHandler::getInstance();
	return true;
}

InputController::~InputController(){
	//SDL_JoystickClose(stick); //Causes issues when deleting...
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
void InputController::update(){

	while(SDL_PollEvent(&cntrlEvent)){
		switch(cntrlEvent.type)
		{
			//occurs whenever a user moves an axis on the joystick. (incl. triggers)
		case SDL_JOYAXISMOTION:
			{
				switch (cntrlEvent.jaxis.axis)
				{
					//X-axis of left joystick
				case 0:
					{						
						leftAnaX = cntrlEvent.jaxis.value;
						normLeftAnaX = leftAnaX/MAX_AXIS_VAL;
						break;
					}
					//Y-Axis of left joystick
				case 1:
					{
						leftAnaY = cntrlEvent.jaxis.value;
						normLeftAnaY = leftAnaY/MAX_AXIS_VAL;
						break;
					}
					//Trigger Buttons
				case 2:
					{
						triggers = cntrlEvent.jaxis.value;
						normTriggers = triggers/MAX_AXIS_VAL;
						break;
					}								
				}
			break;
			}
		//Handles when buttons are pressed down
		case SDL_JOYBUTTONDOWN:
			{
				switch(cntrlEvent.jbutton.button)
				{
					//A is pressed down
				case 0:
					{
						A = true;
						break;
					}
					//B is pressed down
				case 1:
					{
						B = true;
						break;
					}

					//X button is pressed down.
				case 2:
					{						
						Event* ev = new ButtonEvent(2);
						e->emitEvent(ev);
						X = true;
						break;
					}
					//Y was pressed down.
				case 3:
					{
						Y = true;
						break;
					}
					//Left bumper was pressed down
				case 4:
					{
						leftBump = true;
						break;
					}
					//Right bumper was pressed down
				case 5:
					{
						rightBump = true;
						break;
					}
					//Start button was pressed down
				case 7:
					{
						start = true;
						break;
					}
				}
			break;
			}
		//Handles when buttons are released.
		case SDL_JOYBUTTONUP:
			{
				switch(cntrlEvent.jbutton.button)
				{
					//A is pressed down
				case 0:
					{
						A = false;
						break;
					}
					//B is pressed down
				case 1:
					{
						B = false;
						break;
					}

					//X button is pressed down.
				case 2:
					{
						X = false;
						break;
					}
					//Y was pressed down.
				case 3:
					{
						Y = false;
						break;
					}
					//Left bumper was pressed down
				case 4:
					{
						leftBump = false;
						break;
					}
					//Right bumper was pressed down
				case 5:
					{
						rightBump = false;
						break;
					}
					//Start button was pressed down
				case 7:
					{
						start = false;
						break;
					}
				}
			break;
			}
		}
	}

}



Sint16 InputController::getLeftAnalogStickX(){update(); return leftAnaX;}

Sint16 InputController::getLeftAnalogStickY(){update(); return leftAnaY;}

Sint16 InputController::getRightAnalogStickX(){update(); return rightAnaX;}

Sint16 InputController::getRightAnalogStickY(){update(); return rightAnaY;}

double InputController::getNormalizedLeftAnalogStickX(){update(); return normLeftAnaX;}

double InputController::getNormalizedLeftAnalogStickY(){update(); return normLeftAnaY;}

double InputController::getNormalizedRightAnalogStickX(){update(); return normRightAnaX;}

double InputController::getNormalizedRightAnalogStickY(){update(); return normRightAnaY;}

Sint16 InputController::getTriggers(){update(); return triggers;}

double InputController::getNormalizedTriggers(){update(); return normTriggers;}

bool InputController::isADown(){update(); return A;}

bool InputController::isBDown(){update(); return B;}

bool InputController::isXDown(){update(); return X;}

bool InputController::isYDown(){update(); return Y;}

bool InputController::isButtonDown(Button in){
	update();
	switch(in){
		case A_button: {return A;}
		case B_button: {return B;}
		case X_button: {return X;}
		case Y_button: {return Y;}
	}
	return false;
}
;