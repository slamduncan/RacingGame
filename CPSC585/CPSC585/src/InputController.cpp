#include "InputController.h"

#include <iostream>
#include "EventSystemHandler.h"
#include "ButtonEvent.h"
#include "TriggerEvent.h"
#include "RightAnalogEvent.h"
#include "LeftAnalogEvent.h"
#define MAX_AXIS_VAL 32767.0
#define TRIGGER_LIMIT 4000

InputController::InputController()
{
	A = B = X = Y = rightBump = leftBump = start = rightAnaButton = leftAnaButton = false;
	leftAnaX = leftAnaY = rightAnaX = rightAnaY = triggers = 0;
	normLeftAnaX = normLeftAnaY = normRightAnaX = normRightAnaY = normTriggers =  0.0;
//	triggerEvent = new TriggerEvent(0, 0);
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
	 evSys = EventSystemHandler::getInstance();
	return true;
}

InputController::~InputController(){
	//SDL_JoystickClose(stick); //Causes issues when deleting...
}


void InputController::emitTriggers(){
//	triggerEvent->setValue(triggers);
//	triggerEvent->setNormValue(normTriggers);
	TriggerEvent* e = new TriggerEvent(triggers, normTriggers);
	evSys->emitEvent(e);
	delete e;
}

void InputController::emitButtons(){
	
	if (A){
		ButtonEvent* e = new ButtonEvent(A_button);
		evSys->emitEvent(e);
		delete e;
	}	
	
	if (X){
		ButtonEvent* e = new ButtonEvent(X_button);
		evSys->emitEvent(e);
		delete e;
	}
	
	
	if (Y){
		ButtonEvent* e = new ButtonEvent(Y_button);
		evSys->emitEvent(e);
		delete e;
	}
	
	if (B){
		ButtonEvent* e = new ButtonEvent(B_button);
		evSys->emitEvent(e);
		delete e;
	}
}

void InputController::emitLeftAnalog(){
	LeftAnalogEvent* e =  new LeftAnalogEvent(leftAnaX, normLeftAnaX, leftAnaY, normLeftAnaY, leftAnaButton);
	evSys->emitEvent(e);
	delete e;
}
void InputController::emitRightAnalog(){
	RightAnalogEvent* e =  new RightAnalogEvent(rightAnaX, normRightAnaX, rightAnaY, normRightAnaY, rightAnaButton);
	evSys->emitEvent(e);
	delete e;
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
bool triggerInputLimit(int triggerVal){
	return (triggerVal > TRIGGER_LIMIT || 
		triggerVal < -TRIGGER_LIMIT);
}

void InputController::update(SDL_Event cntrlEvent){

	//while(SDL_PollEvent(&cntrlEvent)){
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
						if (triggerInputLimit(cntrlEvent.jaxis.value)){
							leftAnaX = cntrlEvent.jaxis.value;						
							normLeftAnaX = leftAnaX/MAX_AXIS_VAL;
						}
						else {
							leftAnaX = 0;
							normLeftAnaX = 0;
						}
						break;
					}
					//Y-Axis of left joystick
				case 1:
					{
						if (triggerInputLimit(cntrlEvent.jaxis.value)){
						leftAnaY = cntrlEvent.jaxis.value;
						normLeftAnaY = leftAnaY/MAX_AXIS_VAL;
						}
						else {
							leftAnaY = 0;
							normLeftAnaY = 0;
						}
						break;
					}
					//Trigger Buttons
				case 2:
					{
						if (triggerInputLimit(cntrlEvent.jaxis.value)){
							triggers = cntrlEvent.jaxis.value;
							normTriggers = triggers/MAX_AXIS_VAL;
							//evSys->emitEvent(new TriggerEvent(triggers, normTriggers));
						}
						else {
							triggers = 0;
							normTriggers = 0;
						}
						break;
					}
					//Y Axis on the right hand joystick
				case 3:
					{
						if (triggerInputLimit(cntrlEvent.jaxis.value)){
							rightAnaY = cntrlEvent.jaxis.value;
							normRightAnaY = rightAnaY/MAX_AXIS_VAL;
						}
						else{rightAnaY = 0; normRightAnaY = 0;}
						break;
					}
					//X Axis on the right hand joystick
				case 4:
					{
						if (triggerInputLimit(cntrlEvent.jaxis.value)){
							rightAnaX = cntrlEvent.jaxis.value;
							normRightAnaX = rightAnaX/MAX_AXIS_VAL;
						}
						else{rightAnaX = 0; normRightAnaX = 0;}
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
				case A_button:
					{
						Event* ev = new ButtonEvent(A_button);
						evSys->emitEvent(ev);
						delete ev;
						A = true;
						break;
					}
					//B is pressed down
				case B_button:
					{
						Event* ev = new ButtonEvent(B_button);
						evSys->emitEvent(ev);
						delete ev;
						B = true;
						break;
					}

					//X button is pressed down.
				case X_button:
					{						
						Event* ev = new ButtonEvent(X_button);
						evSys->emitEvent(ev);
						delete ev;
						X = true;
						break;
					}
					//Y was pressed down.
				case Y_button:
					{
						Event* ev = new ButtonEvent(Y_button);
						evSys->emitEvent(ev);
						delete ev;
						Y = true;
						break;
					}
					//Left bumper was pressed down
				case L_Bump:
					{
						Event* ev = new ButtonEvent(L_Bump);
						evSys->emitEvent(ev);
						delete ev;
						leftBump = true;
						break;
					}
					//Right bumper was pressed down
				case R_Bump:
					{
						Event* ev = new ButtonEvent(R_Bump);
						evSys->emitEvent(ev);
						delete ev;
						rightBump = true;
						break;
					}
					//Start button was pressed down
				case Start_button:
					{
						Event* ev = new ButtonEvent(Start_button);
						evSys->emitEvent(ev);
						delete ev;
						start = true;
						break;
					}
				case R_Analog:
					{
						rightAnaButton = true;
						break;
					}
				case L_Analog:
					{
						leftAnaButton = true;
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
				case A_button:
					{
						A = false;
						break;
					}
					//B is pressed down
				case B_button:
					{
						B = false;
						break;
					}

					//X button is pressed down.
				case X_button:
					{
						X = false;
						break;
					}
					//Y was pressed down.
				case Y_button:
					{
						Y = false;
						break;
					}
					//Left bumper was pressed down
				case L_Bump:
					{
						leftBump = false;
						break;
					}
					//Right bumper was pressed down
				case R_Bump:
					{
						rightBump = false;
						break;
					}
					//Start button was pressed down
				case Start_button:
					{
						start = false;
						break;
					}
				case R_Analog:
					{
						rightAnaButton = false;
						break;
					}
				case L_Analog:
					{
						leftAnaButton = false;
						break;
					}
				}
			break;
			}
		}
	//}

}



Sint16 InputController::getLeftAnalogStickX(){/*update();*/ return leftAnaX;}

Sint16 InputController::getLeftAnalogStickY(){/*update();*/ return leftAnaY;}

Sint16 InputController::getRightAnalogStickX(){/*update();*/ return rightAnaX;}

Sint16 InputController::getRightAnalogStickY(){/*update();*/ return rightAnaY;}

double InputController::getNormalizedLeftAnalogStickX(){/*update();*/ return normLeftAnaX;}

double InputController::getNormalizedLeftAnalogStickY(){/*update();*/ return normLeftAnaY;}

double InputController::getNormalizedRightAnalogStickX(){/*update();*/ return normRightAnaX;}

double InputController::getNormalizedRightAnalogStickY(){/*update();*/ return normRightAnaY;}

Sint16 InputController::getTriggers(){/*update();*/ return triggers;}

double InputController::getNormalizedTriggers(){/*update();*/ return normTriggers;}

bool InputController::isADown(){/*update();*/ return A;}

bool InputController::isBDown(){/*update();*/ return B;}

bool InputController::isXDown(){/*update();*/ return X;}

bool InputController::isYDown(){/*update();*/ return Y;}

bool InputController::isButtonDown(Button in){
	/*update();*/
	switch(in){
		case A_button: {return A;}
		case B_button: {return B;}
		case X_button: {return X;}
		case Y_button: {return Y;}
		case R_Bump: {return rightBump;}
		case L_Bump: {return leftBump;}
		case Start_button: {return start;}
	}
	return false;
}
;