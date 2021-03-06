#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H

#include "SDL.h"
#include "EventSystemHandler.h"

/* Sample use of InputController:
	
	InputController c = InputController();
	bool continueOn = c.initialize(ControllerIndex);  (NOTE: indexing of controllers start at 0)
	if (!continueOn){ "CONTROLLER DID NOT INITIALIZE PROPERLY  }
*/

class InputController
{
private:
	bool A, B, X, Y, rightBump, leftBump, start, rightAnaButton, leftAnaButton, backButton;
	Sint16 leftAnaX, leftAnaY, rightAnaX, rightAnaY, triggers;
	double normLeftAnaX, normLeftAnaY, normRightAnaX, normRightAnaY, normTriggers;
	int InputControllerIdentifier;
	SDL_Event cntrlEvent;
	SDL_Joystick *stick;
	//TriggerEvent *triggerEvent;
	//void update();
	
	EventSystemHandler* evSys;

public:		
	void update(SDL_Event cntrlEvent);
	void emitTriggers();
	void emitButtons();
	void emitLeftAnalog();
	void emitRightAnalog();
	static void initSDLJoystick();
	enum Button {A_button, B_button, X_button, Y_button, L_Bump, R_Bump, Back_button, Start_button, L_Analog, R_Analog};
	InputController();
	~InputController();
		
	bool initialize(int controllerIndex);	
	bool isADown();
	bool isBDown();
	bool isXDown();
	bool isYDown();
	bool isButtonDown(Button in); //returns true if the button is pressed down, false otherwise (including if input is not valid).
	bool rightBumperIsDown();
	bool leftBumperIsDown();

	Sint16 getLeftAnalogStickX();
	Sint16 getLeftAnalogStickY();
	Sint16 getRightAnalogStickX();
	Sint16 getRightAnalogStickY();
	Sint16 getTriggers();	

	double getNormalizedLeftAnalogStickX();
	double getNormalizedRightAnalogStickX();
	double getNormalizedLeftAnalogStickY();
	double getNormalizedRightAnalogStickY();
	double getNormalizedTriggers();
	
};

#endif