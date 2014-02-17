#pragma once
#include "Block.h"
namespace Blocks 
{
	class Dirt : public Block
	{
	public:
		Dirt(void);
		~Dirt(void);
		inline bool IsOpaque();
		float GetFaceTextureId(FaceType face);
	};
	
	bool Dirt::IsOpaque() { return true; }
}
