#include <iostream>
#include <fstream>
#include <SDL.h>
//#pragma comment(lib,"SDL.lib") 
//#pragma comment(lib,"SDLmain.lib") 
//#include "C:\Program Files\CPSC585\SDL\SDL-1.2.14\include\SDL_joystick.h"
using namespace std;

int main(int argc, char *argv[])
{
if (SDL_Init(SDL_INIT_JOYSTICK) < 0 )
{
	fprintf(stderr, "Couldn't inititalize SDL: %s\n", SDL_GetError());
	exit(1);
}

	int i;
	cin >> i;
	cout << i;
//SDL_Quit();
}