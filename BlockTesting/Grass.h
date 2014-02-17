#pragma once
#include "block.h"
namespace Blocks 
{
	class Grass :
		public Block
	{
	public:
		Grass(void);
		~Grass(void);
		inline bool IsOpaque();
		float GetFaceTextureId(FaceType face);
	};
	bool Grass::IsOpaque() { return true; }
}
