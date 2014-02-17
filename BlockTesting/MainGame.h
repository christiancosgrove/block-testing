#pragma once
#include <GL\glew.h>
#include <SFML\Graphics.hpp>
#include <SFML\Graphics\RenderStates.hpp>
#include "World.h"
#include "GLManager.h"

class MainGame
{
public:
	static const int FRAME_LIMIT = 120; //max FPS
	MainGame(void);
	~MainGame(void);
	void Draw();
	sf::RenderWindow Window;
	void HandleWindowChanges();
	std::string ExePath();
private:
	void initGL();
	World world;
	GLManager glManager;
	sf::Font font;
	std::clock_t time;
	int frames;
};

