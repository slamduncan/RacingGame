#ifndef MENU_H
#define MENU_H

#include "SDL.h"
#include "Renderer.h"

class Menu 
{
public:	
	int run(Renderer *ren);
	void loading(Renderer *ren, std::string loadingWhat);
	enum SELECTABLE {START, QUIT, NUM_OF_OPTIONS};
	int timeScreen(Renderer *ren);

private:
	bool triggerInputLimit(int triggerVal);

};

#endif