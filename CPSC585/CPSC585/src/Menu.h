#ifndef MENU_H
#define MENU_H

#include "SDL.h"
#include "Renderer.h"

class Menu 
{
public:	
	int run(Renderer *ren);
	void loading(Renderer *ren, std::string loadingWhat);
	int inGameMenu(Renderer *ren);
	enum SELECTABLE {START, QUIT, NUM_OF_OPTIONS};
	enum IN_GAME_MENU_OPTIONS{CONTINUE, QUIT_IN_GAME, NUM_OF_IN_GAME_OPTIONS};
	int timeScreen(Renderer *ren);

private:
	bool triggerInputLimit(int triggerVal);

};

#endif