#pragma once
#include <glm\glm.hpp>
#include "Chunk.h"
//handles terrain generation of chunks
class WorldGenerator
{
public:
	//world seed
	static const int SEED = 28713419727;
	WorldGenerator(void);
	~WorldGenerator(void);
	//generate terrain of chunk reference
	static void GenerateChunkData(Chunk& chunk); 
private:
	//deterministic pseudo-random number generation based on a specific seed
	//allows smooth chunk boundaries
	static int randNumGen(int baseSeed, int x);
	//get vector representing gradient value for Perlin noise
	static glm::vec2 getVector(int seed, short x, short y);
	//used in the generation of higher-scaled noise (additional octaves)
	static inline int positiveMod(int i, int n);
};

int WorldGenerator::positiveMod(int i, int n)
{
	return i & (n - 1);
}
