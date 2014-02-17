#pragma once
#include<utility>

enum FaceType
{
	TOP, BOTTOM, LEFT, RIGHT, FRONT, BACK
};

enum BlockType
{
	AIR, STONE, DIRT, GRASS, WATER
};

class Block
{
public:

	static const int NUM_TEX_WIDTH = 16;
	Block(void);
	~Block(void);
	static Block* Blocks[];
	virtual inline bool IsOpaque() = 0;
		

	/* Block texture definitions */
	virtual float GetFaceTextureId(FaceType face) = 0;

}; 
