#pragma once
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>

class Camera
{
public:
	Camera(int windowWidth, int windowHeight);
	Camera(glm::vec3);
	Camera(glm::vec3, glm::vec3);
	~Camera(void);
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetTransformMatrix();
	glm::vec3 GetViewDirection();
	glm::vec3 Position;
	float XRotation;
	float ZRotation;
	float FieldOfView;
	void ResizedWindow(int windowWidth, int windowHeight);
	const float NEAR_PLANE;
	const float FAR_PLANE;
private:
	float aspectRatio;
};

