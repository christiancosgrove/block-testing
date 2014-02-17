#pragma once
#include "Block.h"

namespace Blocks
{
	class Air : public ::Block
	{
	public:
		Air(void);
		~Air(void);
		inline bool IsOpaque();
		float GetFaceTextureId(FaceType face);
	};

	bool Air::IsOpaque() { return false; }
}

