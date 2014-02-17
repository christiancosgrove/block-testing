#pragma once
#include "GL\glew.h"
#include <SFML\OpenGL.hpp>
#include <vector>
#include "Block.h"
#include "BlockCoord.h"
#include "HashHelper.h"
#include <unordered_map>
#include <mutex>


//vertex data to pass to GL
struct Vertex 
{
	float x, y, z, u, v, textureId, brightness;
	unsigned char face;
	Vertex() : x(0), y(0), z(0), u(0), v(0) { }
	Vertex(float _x, float _y, float _z, float _u, float _v, float _textureId, unsigned char _face, float _brightness = 1.0f) : 
		x(_x), y(_y), z(_z), u(_u), v(_v), textureId(_textureId), brightness(_brightness), face(_face)
	{
	}
	~Vertex()
	{
	}
};

//simple representation of a chunk's location
struct ChunkCoord
{
	int x;
	int y;
	int z;

	ChunkCoord(int _x, int _y) : x(_x), y(_y), z(0) { }
	ChunkCoord(int _x, int _y, int _z) : x(_x), y(_y), z(_z) { }

	bool ChunkCoord::operator==(const ChunkCoord& c) const
	{
		return (x == c.x && y==c.y && z==c.z);
	}
};

class Chunk
{
public:
	static const int WIDTH = 16;

	Chunk(ChunkCoord coord, std::unordered_map<ChunkCoord, Chunk*>& _chunkMap);
	~Chunk(void);
	//block id array -- block data
	std::vector<unsigned char> BlockData;
	//draw indexed elements in vertexData
	unsigned long Draw();
	//returns pointer to reference instance of block -- allows for opacity/property checking
	inline Block* GetBlockTypeAt(int, int, int);
	inline Block* GetBlockTypeAt(BlockCoord);
	//returns int block id at block coord
	inline int BlockAt(int, int, int);
	inline int BlockAt(BlockCoord);
	//x,y,z coordinates of chunk in chunk space
	const ChunkCoord chunkCoord;
	//pushes faces to vertexData by reading BlockData
	void GenerateVertexData();
	//experimental -- deprecated for now
	void NearbyChunkGeneration(Chunk&);
	//terrain generation
	void GenerateChunkBlockData();
	//externally determining state of chunk; experimental
	bool IsGeneratedBlocks;
	bool IsGeneratedVertexData;
	bool IsGeneratedVBO;

	std::mutex ChunkActionMutex;
	//generates VBO and associated GL stuff
	void GenerateVBO();

	//thread for worldgen/vertex generation
	//TODO: improve asynchronousness
private:

	std::unordered_map<ChunkCoord, Chunk>& chunkMap;
	//reference to world's chunk unordered_map
	
	//contain data for drawing elements
	std::vector<Vertex> vertexData;
	std::vector<unsigned short> indexData;

	GLuint vertexBufferObject;
	GLuint indexBufferObject;
	//from chunkmap, get 6 nearby chunks
	void getNearbyChunkList(Chunk**, std::unordered_map<ChunkCoord, Chunk*>&);
	//adds face to vertex data
	void pushFace(BlockCoord, FaceType, Chunk**);

	//Gets block type relative to chunk coord*WIDTH (includes 6 nearby chunks)
	inline Block* getBlockTypeRelative(BlockCoord, Chunk**);
	//TODO: implement corners for better ambient occlusion
	//http://0fps.wordpress.com/2013/07/03/ambient-occlusion-for-minecraft-like-worlds/
	inline float vertexAO(bool side1, bool side2, bool corner); //vertex ambient occlusion function


	
};

namespace std
{
	//implementation of chunkcoord hashing -- for std::unordered_map<ChunkCoord, Chunk*>
	template<> struct hash<ChunkCoord> 
	{
		inline size_t operator()(const ChunkCoord& v) const
		{
			size_t seed = 0;
			::hash_combine(seed, v.x);
			::hash_combine(seed, v.y);
			::hash_combine(seed, v.z);
			return seed;
		}
	};
}

int Chunk::BlockAt(int x, int y, int z)
{
	return x + y*WIDTH + z*WIDTH*WIDTH;
}
int Chunk::BlockAt(BlockCoord c)
{
	return c.x + c.y*WIDTH + c.z*WIDTH*WIDTH;
}
Block* Chunk::GetBlockTypeAt(int x, int y, int z)
{
	return Block::Blocks[BlockData[BlockAt(x,y,z)]];
}
Block* Chunk::GetBlockTypeAt(BlockCoord c)
{
	return Block::Blocks[BlockData[BlockAt(c)]];
}

Block* Chunk::getBlockTypeRelative(BlockCoord coord, Chunk** nearbyChunks)
{

	if (coord.z>WIDTH - 1 && coord.x<WIDTH && coord.x>=0 && coord.y<WIDTH && coord.y>=0)
	{
		if (nearbyChunks[0]!=nullptr)
			return nearbyChunks[0]->GetBlockTypeAt(coord.x, coord.y, 0);
		else return Block::Blocks[AIR];
	}
	else if (coord.z<0 && coord.x<WIDTH && coord.x>=0 && coord.y<WIDTH && coord.y>=0)
	{
		if (nearbyChunks[1]!=nullptr)
			return nearbyChunks[1]->GetBlockTypeAt(coord.x, coord.y, WIDTH-1);
		else return Block::Blocks[AIR];
	}
	else if (coord.x>WIDTH - 1 && coord.z<WIDTH && coord.z>=0 && coord.y<WIDTH && coord.y>=0)
	{
		if (nearbyChunks[2]!=nullptr)
			return nearbyChunks[2]->GetBlockTypeAt(0, coord.y, coord.z);
		else return Block::Blocks[AIR];
	}
	else if (coord.x<0 && coord.z<WIDTH && coord.z>=0 && coord.y<WIDTH && coord.y>=0)
	{
		if (nearbyChunks[3]!=nullptr)
			return nearbyChunks[3]->GetBlockTypeAt(WIDTH - 1, coord.y, coord.z);
		else return Block::Blocks[AIR];
	}
	else if (coord.y>WIDTH - 1 && coord.x<WIDTH && coord.x>=0 && coord.z<WIDTH && coord.z>=0)
	{
		if (nearbyChunks[4]!=nullptr)
			return nearbyChunks[4]->GetBlockTypeAt(coord.x, 0, coord.z);
		else return Block::Blocks[AIR];
	}
	else if (coord.y<0 && coord.x<WIDTH && coord.x>=0 && coord.z<WIDTH && coord.z>=0)
	{
		if (nearbyChunks[5]!=nullptr)
			return nearbyChunks[5]->GetBlockTypeAt(coord.x, WIDTH-1, coord.z);
		else return Block::Blocks[AIR];
	}	
	else if (!(coord.x>=0 && coord.x < WIDTH && coord.y>=0 && coord.y<WIDTH && coord.z>=0 && coord.z<WIDTH))
		return Block::Blocks[AIR];
	else return GetBlockTypeAt(coord);
}

float Chunk::vertexAO(bool side1, bool side2, bool corner)
{
	if (side1 && side2)
		return 0;
	return (float)(3 - (side1 + side2 + corner));
}