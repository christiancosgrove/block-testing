#version 330 core
layout(location = 0) in vec3 vertexPosition_modelSpace;
layout(location = 1) in vec3 vertexUV;
layout(location = 2) in float vertexBrightness;
layout(location = 3) in int face;

uniform mat4 viewMatrix;
uniform vec3 lightDir;
uniform vec3 cameraPos;

out vec3 UV;
out vec3 lightDir_tangentSpace;
out vec3 cameraPos_tangentSpace;
out float brightness;

void main(void) 
{
	gl_Position = viewMatrix*(vec4(vertexPosition_modelSpace, 1.0f));
	UV = vertexUV;
	brightness = vertexBrightness;
	mat3 TBN;
	switch (face)
	{
	case 0:
		TBN = mat3(
			0.0, 1.0, 0.0,
			1.0, 0.0, 0.0,
			0.0, 0.0, 1.0);
		break;
	case 1:
		TBN = mat3(
			0.0, -1.0, 0.0,
			-1.0, 0.0, 0.0,
			0.0, 0.0, -1.0);
		break;
	case 2:
		TBN = mat3(
			0.0, 0.0, 1.0,
			0.0, 1.0, 0.0,
			1.0, 0.0, 0.0);
		break;
	case 3:
		TBN = mat3(
			0.0, 0.0, -1.0,
			0.0, -1.0, 0.0,
			-1.0, 0.0, 0.0);
		break;
	case 4:
		TBN = mat3(
			1.0, 0.0, 0.0,
			0.0, 0.0, 1.0,
			0.0, -1.0, 0.0);
		break;
	case 5:
		TBN = mat3(
			-1.0, 0.0, 0.0,
			0.0, 0.0, -1.0,
			0.0, 1.0, 0.0);
		break;
	}

	cameraPos_tangentSpace = TBN*normalize(cameraPos - vertexPosition_modelSpace);
	lightDir_tangentSpace = TBN*normalize(lightDir);
}