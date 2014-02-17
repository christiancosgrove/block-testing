#pragma once
#include "Block.h"

namespace Blocks
{
	class Stone : public Block
	{
	public:
		Stone(void);
		~Stone(void);
		inline bool IsOpaque();
		float GetFaceTextureId(FaceType face);
	};

	bool Stone::IsOpaque() { return true; }
}
