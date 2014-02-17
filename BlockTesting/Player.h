#pragma once
#include "Camera.h"
#include "SFML\Window.hpp"

class Player
{
public:
	Player(int windowWidth, int windowHeight);
	~Player(void);
	Camera Camera;
	glm::vec3 LastChunkLoadPos;
	void Update();
private:
	float playerSpeed;
	bool mouseFocus;
};

