#include "Player.h"
#include "MainGame.h"
#define M_PI 3.14159265359f

Player::Player(int windowWidth, int windowHeight) : 
	Camera(windowWidth, windowHeight), 
	playerSpeed(1.0f), 
	mouseFocus(true),
	LastChunkLoadPos(glm::vec3())
{
}


Player::~Player(void)
{
}

void Player::Update()
{
	float shiftSpeedFactor = (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) ? 45.0f : 1.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		Camera.Position+=Camera.GetViewDirection() * playerSpeed * (shiftSpeedFactor / MainGame::FRAME_LIMIT);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		Camera.Position-=Camera.GetViewDirection() * playerSpeed * (shiftSpeedFactor / MainGame::FRAME_LIMIT);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		Camera.Position+=glm::vec3(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) * glm::eulerAngleXZ(Camera.XRotation, Camera.ZRotation)) * playerSpeed * (shiftSpeedFactor / MainGame::FRAME_LIMIT);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		Camera.Position-=glm::vec3(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) * glm::eulerAngleXZ(Camera.XRotation, Camera.ZRotation)) * playerSpeed * (shiftSpeedFactor / MainGame::FRAME_LIMIT);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
		mouseFocus = !mouseFocus;
	if (mouseFocus)
	{
		Camera.XRotation += (float)(sf::Mouse::getPosition().y - 540)/200;
		if (Camera.XRotation>0)
			Camera.XRotation=0;
		if (Camera.XRotation<-M_PI)
			Camera.XRotation=-M_PI;
		Camera.ZRotation += (float)(sf::Mouse::getPosition().x - 960)/200;
		sf::Mouse::setPosition(sf::Vector2<int>(960,540));
	}
}