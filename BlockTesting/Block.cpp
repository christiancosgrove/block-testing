#include "Block.h"
/*Block references*/
#include "Air.h"
#include "Stone.h"
#include "Dirt.h"
#include "Grass.h"
#include "Water.h"

Block::Block(void)
{
}


Block::~Block(void)
{
	
}

Block* Block::Blocks[] = { new Blocks::Air(), new Blocks::Stone(), new Blocks::Dirt(), new Blocks::Grass(), new Blocks::Water() };

