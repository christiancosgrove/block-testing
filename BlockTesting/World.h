#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Player.h"
#include <unordered_map>
#include <vector>
#include <deque>
#include "Chunk.h"
#include <GL\glew.h>
#include <SFML\OpenGL.hpp>
#include "GLManager.h"
#include "TextureManager.h"
#include "ThreadPool.h"

enum ChunkActionType
{
	GENERATE_BLOCK_DATA,
	GENERATE_VERTEX_DATA,
	NEW_CHUNK,
	DRAW,
	UPDATE,
	REMOVE,
	WORLD_DISPOSED
};

struct ChunkAction
{
	ChunkActionType type;
	ChunkCoord coord;

	ChunkAction() : coord(ChunkCoord(0,0,0))
	{

	}

	ChunkAction(ChunkActionType _type, ChunkCoord _coord) : type(_type), coord(_coord)
	{

	}
};

class World
{
public:
	World(void);
	~World(void);
	void Draw();
	void PrepareToDraw(GLManager);
	void ResizedWindow(int windowWidth, int windowHeight);
	static const int DEFAULT_WINDOW_WIDTH=1920;
	static const int DEFAULT_WINDOW_HEIGHT=1080;
	static const int RENDER_DISTANCE = 10;
private: 
	std::unordered_map<ChunkCoord, Chunk*> chunks;
	TextureManager textureManager;
	void generateChunks();
	void generateChunk(ChunkCoord);
	void addChunk(ChunkCoord);
	Player player;
	GLuint matrixLocation;
	GLuint lightDirLocation;
	GLuint cameraPosLocation;
	int temp;
	float sunAngle;
	void handleChunks();
	void newChunkAction(ChunkActionType, ChunkCoord);
	std::mutex chunkMapMutex;
	std::mutex chunkMutex;
	std::condition_variable chunkCV;
	bool worldDisposed;
	bool chunksMustBeGenerated;
};

