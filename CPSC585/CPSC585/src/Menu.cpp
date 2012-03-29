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
	unsigned int delay = 120;
	Uint32 lastTickCount = SDL_GetTicks();
	bool moved = false;
	while(InMenu)
	{				
		ren->clearGL();
		ren->changeFontSize(72);
		ren->outputText("Varios", 255, 255, 255,550, 600);
		ren->changeFontSize(26);
		switch(currentSelection)
		{
		case(QUIT):
			{
				ren->outputText("S t a r t", 255, 0,0, 1280/2, 720/2);
				ren->outputText("Q u i t", 0, 255,0, 1280/2, 720/3);
				break;
			}
		case(START):
			{
				ren->outputText("S t a r t", 0, 255,0, 1280/2, 720/2);
				ren->outputText("Q u i t", 255, 0,0, 1280/2, 720/3);
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
					if (eventIn.jbutton.button == 6)
					{
						currentSelection= (SELECTABLE) ((currentSelection - 1 + NUM_OF_OPTIONS) % NUM_OF_OPTIONS);
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
	while(!goBack)
	{
	ren->glEnable2D();
	ren->clearGL();
	
	int position = 1;
	int offset = 10;
	for (int i = 0; i < entMan->numCars(); i++)
	{	
		for (int j = 0; j < entMan->numCars(); j++)
		{	
			Car* tempC = entMan->getCar(j);
			if (tempC->finalPosition == position)
			{
				ren->changeFontSize(26);
				std::stringstream name;
				if (tempC->id == 0)
					name << "You!\n";
				else
					name << "Computer " << tempC->id << "\n";				
				if (name.str().length() > 20)
					offset += name.str().length()*16 + 16;
				else
					offset += 20*16 + 16;
				ren->outputText(name.str(), 0, 255, 255, offset, 720/2);
				ren->changeFontSize(16);				
				ren->outputText(tempC->timeFinished.str(), 0, 255, 150,offset, 720/3);
				position++;
			}
		}		
	}
//	for (int i = 0; i < entMan->numCars();i++)
	{
		for (int j = 0; j < entMan->numCars(); j++)
		{
			Car* tempC = entMan->getCar(j);
			if (!tempC->finishedRacing)
			{
				
				ren->changeFontSize(26);
				std::stringstream name;
				name << "Computer " << tempC->id << "\n";
				if (name.str().length() > 20)
					offset += name.str().length()*16 + 16;
				else
					offset += 20*16 + 16;
				ren->outputText(name.str(), 0, 255, 255, offset, 720/2);
				ren->changeFontSize(16);				
				ren->outputText(tempC->timeFinished.str(), 0, 255, 150, offset, 720/3);
				position++;
			}
		}
	}

	ren->outputText("Press Start To Go Back to Main Menu", 255, 0,0, 1280/3, 680);
	ren->glDisable2D();		
	SDL_Event eventIn;	
	bool goBack = false;
	
	while(SDL_PollEvent( &eventIn )) {
		
		switch (eventIn.type)
		{			
		case SDL_JOYBUTTONDOWN:
			{
				if (eventIn.jbutton.button == 7){ //Start
					goBack = true;
				}
			}
			break;
		}		
	}
	ren->updateGL();
	}
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
int Menu::inGameMenu(Renderer *ren)
{
	IN_GAME_MENU_OPTIONS currentSelection = CONTINUE;
	bool InGameMenu = true;
	ren->glEnable2D();
	ren->changeFontSize(26);
	unsigned int delay = 120;
	Uint32 lastTickCount = SDL_GetTicks();
	bool moved = false;
	while(InGameMenu)
	{				
		ren->clearGL();
		switch(currentSelection)
		{
		case(QUIT_IN_GAME):
			{
				ren->outputText("Resume", 255, 0,0, 1280/2, 720/2);
				ren->outputText("Quit", 0, 255,0, 1280/2, 720/3);
				break;
			}
		case(CONTINUE):
			{
				ren->outputText("Resume", 0, 255,0, 1280/2, 720/2);
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
								currentSelection = (IN_GAME_MENU_OPTIONS) ((currentSelection + 1) % NUM_OF_IN_GAME_OPTIONS);
							}
							else
							{
								currentSelection= (IN_GAME_MENU_OPTIONS) ((currentSelection - 1 + NUM_OF_IN_GAME_OPTIONS) % NUM_OF_IN_GAME_OPTIONS);
							}
						}
					}					
				}
				break;

			case SDL_JOYBUTTONDOWN:
				{
					if (eventIn.jbutton.button == 7){ //Start
						InGameMenu = false;
					}
					if (eventIn.jbutton.button == 6)
					{
						currentSelection= (IN_GAME_MENU_OPTIONS) ((currentSelection - 1 + NUM_OF_IN_GAME_OPTIONS) % NUM_OF_IN_GAME_OPTIONS);
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