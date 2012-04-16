#ifndef MENU_H
#define MENU_H

#include "SDL.h"
#include "Renderer.h"

class Menu 
{
public:	
	int run(Renderer *ren);
	void loading(Renderer *ren, std::string loadingWhat, bool wait = false);
	int inGameMenu(Renderer *ren);
	enum SELECTABLE {START, QUIT, NUM_OF_OPTIONS};
	enum IN_GAME_MENU_OPTIONS{CONTINUE, QUIT_IN_GAME, NUM_OF_IN_GAME_OPTIONS};
	int timeScreen(Renderer *ren);
	void freeingScreen(Renderer *ren, std::string loadingWhat);

private:
	bool triggerInputLimit(int triggerVal);

};

#endif