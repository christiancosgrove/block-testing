#pragma once
#include "block.h"

namespace Blocks
{
	class Water :
		public Block
	{
	public:
		Water(void);
		~Water(void);
		inline bool IsOpaque() { return false; }
		float GetFaceTextureId(FaceType face);
	};
}