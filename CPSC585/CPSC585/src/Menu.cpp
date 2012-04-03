#include "Menu.h"


bool Menu::triggerInputLimit(int triggerVal){
	return (triggerVal > 8000 || 
		triggerVal < -8000);
}

int Menu::run(Renderer *ren)
{	
	SELECTABLE currentSelection = START;
	bool InMenu = true;
	ren->glEnable2D();
	ren->changeFontSize(26);
	int delay = 100;
	Uint32 lastTickCount = SDL_GetTicks();
	bool moved = false;
	while(InMenu)
	{				
		ren->clearGL();
		switch(currentSelection)
		{
		case(QUIT):
			{
				ren->outputText("Start", 255, 0,0, 1280/2, 720/2);
				ren->outputText("Quit", 0, 255,0, 1280/2, 720/3);
				break;
			}
		case(START):
			{
				ren->outputText("Start", 0, 255,0, 1280/2, 720/2);
				ren->outputText("Quit", 255, 0,0, 1280/2, 720/3);
				break;
			}
		}

		if (delay < SDL_GetTicks() - lastTickCount)
		{
			lastTickCount = SDL_GetTicks();
			moved = false;			
		}		
		SDL_Event eventIn;		
		while(SDL_PollEvent( &eventIn )) {
			
			switch (eventIn.type)
			{			
			case SDL_JOYAXISMOTION:
				{
					if (moved)
					{						
						break;
					}
					moved = true;
					//Y-Axis of left joystick
					if (eventIn.jaxis.axis == 1)
					{
						if (triggerInputLimit(eventIn.jaxis.value)){
							if (eventIn.jaxis.value > 0)
							{
								currentSelection = (SELECTABLE) ((currentSelection + 1) % NUM_OF_OPTIONS);
							}
							else
							{
								currentSelection= (SELECTABLE) ((currentSelection - 1 + NUM_OF_OPTIONS) % NUM_OF_OPTIONS);
							}
						}
					}					
				}
				break;

			case SDL_JOYBUTTONDOWN:
				{
					if (eventIn.jbutton.button == 7){ //Start
						InMenu = false;
					}
				}
				break;
			}
		}
		ren->updateGL();
	}
	ren->glDisable2D();
	return currentSelection;
}

int Menu::timeScreen(Renderer *ren)
{
	EntityManager* entMan = EntityManager::getInstance();
	ren->glEnable2D();
	ren->clearGL();
	
	for (int i = 0; i < entMan->numCars(); i++)
	{	
		ren->changeFontSize(56);
		std::stringstream name;
		if (i == 0)
			name << "You!\n";
		else
			name << "Computer " << i << "\n";
		ren->outputText(name.str(), 0, 255, 255, 1280/3, 720/2);
		ren->changeFontSize(26);
		
		ren->outputText("", 0, 255, 255, 1280/2, 720/3);				
	}
	ren->updateGL();
	ren->glDisable2D();
	return 0;
}

void Menu::loading(Renderer *ren, std::string loadingWhat)
{
	ren->glEnable2D();
	ren->clearGL();
	ren->changeFontSize(26);
	ren->outputText("Loading...", 0, 255, 255, 1280/2, 720/2);
	ren->outputText(loadingWhat, 0, 255, 255, 1280/2, 720/3);
	ren->updateGL();
	ren->glDisable2D();
}
