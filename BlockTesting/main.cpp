#include "MainGame.h"
#define SFML_STATIC

int main()
{
	MainGame mainGame;
	
	while (mainGame.Window.isOpen())
	{
		mainGame.Draw();
		mainGame.HandleWindowChanges();
	}
	return 0;
}