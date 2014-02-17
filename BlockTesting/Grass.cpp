#include "Grass.h"

namespace Blocks
{
	Grass::Grass(void)
	{
	}


	Grass::~Grass(void)
	{

	}
	
	float Grass::GetFaceTextureId(FaceType face)
	{
		switch (face)
		{
		case TOP:
			return 2.0f;
		case BOTTOM:
			return 1.0f;
		case LEFT:
		case RIGHT:
		case FRONT:
		case BACK:
			return 3.0f;
		}
		return 0.0f;
	}
}