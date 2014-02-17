#include "Chunk.h"
#include "WorldGenerator.h"


Chunk::Chunk(ChunkCoord coord, std::unordered_map<ChunkCoord, Chunk*>& _chunkMap) : 
	chunkCoord(coord),
	IsGeneratedBlocks(false), 
	IsGeneratedVertexData(false),
	IsGeneratedVBO(false),
	chunkMap(_chunkMap),
	BlockData(WIDTH*WIDTH*WIDTH)
{
}


//Terrain generation (basic at moment)
void Chunk::GenerateChunkBlockData()
{
	ChunkActionMutex.lock();
	WorldGenerator::GenerateChunkData(*this);
	IsGeneratedBlocks = true;
	ChunkActionMutex.unlock();
}

Chunk::~Chunk(void)
{
	indexData.clear();
	vertexData.clear();
	indexData.shrink_to_fit();
	vertexData.shrink_to_fit();
}
//surrounding chunk tuple: 
//		[0]: TOP		(+z)
//		[1]: BOTTOM		(-z)
//		[2]: LEFT		(+x)
//		[3]: RIGHT		(-x)
//		[4]: FRONT		(+y)
//		[5]: BACK		(-y)

void Chunk::getNearbyChunkList(Chunk** chunkList, std::unordered_map<ChunkCoord, Chunk*>& chunks)
{
	if (chunks.find(ChunkCoord(chunkCoord.x, chunkCoord.y, chunkCoord.z + 1))!=chunks.end())
		chunkList[0] = chunks[ChunkCoord(chunkCoord.x, chunkCoord.y, chunkCoord.z + 1)]->IsGeneratedBlocks ? chunks[ChunkCoord(chunkCoord.x, chunkCoord.y, chunkCoord.z + 1)] : nullptr;
	else 
		chunkList[0] = nullptr;
	if (chunks.find(ChunkCoord(chunkCoord.x, chunkCoord.y, chunkCoord.z - 1))!=chunks.end())
		chunkList[1] = chunks[ChunkCoord(chunkCoord.x, chunkCoord.y, chunkCoord.z - 1)]->IsGeneratedBlocks ? chunks[ChunkCoord(chunkCoord.x, chunkCoord.y, chunkCoord.z - 1)] : nullptr;
	else 
		chunkList[1] = nullptr;
	if (chunks.find(ChunkCoord(chunkCoord.x + 1, chunkCoord.y, chunkCoord.z))!=chunks.end())
		chunkList[2] = chunks[ChunkCoord(chunkCoord.x + 1, chunkCoord.y, chunkCoord.z)]->IsGeneratedBlocks ? chunks[ChunkCoord(chunkCoord.x + 1, chunkCoord.y, chunkCoord.z)] : nullptr;
	else 
		chunkList[2] = nullptr;
	if (chunks.find(ChunkCoord(chunkCoord.x - 1, chunkCoord.y, chunkCoord.z))!=chunks.end())
		chunkList[3] = chunks[ChunkCoord(chunkCoord.x - 1, chunkCoord.y, chunkCoord.z)]->IsGeneratedBlocks ? chunks[ChunkCoord(chunkCoord.x - 1, chunkCoord.y, chunkCoord.z)] : nullptr;
	else 
		chunkList[3] = nullptr;
	if (chunks.find(ChunkCoord(chunkCoord.x, chunkCoord.y + 1, chunkCoord.z))!=chunks.end())
		chunkList[4] = chunks[ChunkCoord(chunkCoord.x, chunkCoord.y + 1, chunkCoord.z)]->IsGeneratedBlocks ? chunks[ChunkCoord(chunkCoord.x, chunkCoord.y + 1, chunkCoord.z)] : nullptr;
	else 
		chunkList[4] = nullptr;
	if (chunks.find(ChunkCoord(chunkCoord.x, chunkCoord.y - 1, chunkCoord.z))!=chunks.end())
		chunkList[5] = chunks[ChunkCoord(chunkCoord.x, chunkCoord.y - 1, chunkCoord.z)]->IsGeneratedBlocks ? chunks[ChunkCoord(chunkCoord.x, chunkCoord.y - 1, chunkCoord.z)] : nullptr;
	else 
		chunkList[5] = nullptr;
}

void Chunk::GenerateVertexData()
{
	Chunk* chunkList[6];
	getNearbyChunkList(chunkList, chunkMap);

	ChunkActionMutex.lock();
	for (int x = 0; x<Chunk::WIDTH;x++)
	{
		for (int y = 0; y<Chunk::WIDTH;y++)
		{
			for (int z = 0; z<Chunk::WIDTH;z++)
			{
				if (BlockData[BlockAt(x,y,z)]!=0)
				{
					if (!getBlockTypeRelative(BlockCoord(x,y,z+1), chunkList)->IsOpaque() && getBlockTypeRelative(BlockCoord(x,y,z+1), chunkList)!=getBlockTypeRelative(BlockCoord(x,y,z), chunkList))
						pushFace(BlockCoord(x,y,z), TOP, chunkList);
					if (!getBlockTypeRelative(BlockCoord(x,y,z-1), chunkList)->IsOpaque() && getBlockTypeRelative(BlockCoord(x,y,z-1), chunkList)!=getBlockTypeRelative(BlockCoord(x,y,z), chunkList))
						pushFace(BlockCoord(x,y,z), BOTTOM, chunkList);
					if (!getBlockTypeRelative(BlockCoord(x+1,y,z), chunkList)->IsOpaque() && getBlockTypeRelative(BlockCoord(x+1,y,z), chunkList)!=getBlockTypeRelative(BlockCoord(x,y,z), chunkList))
						pushFace(BlockCoord(x,y,z), LEFT, chunkList);
					if (!getBlockTypeRelative(BlockCoord(x-1,y,z), chunkList)->IsOpaque() && getBlockTypeRelative(BlockCoord(x-1,y,z), chunkList)!=getBlockTypeRelative(BlockCoord(x,y,z), chunkList))
						pushFace(BlockCoord(x,y,z), RIGHT, chunkList);
					if (!getBlockTypeRelative(BlockCoord(x,y+1,z), chunkList)->IsOpaque() && getBlockTypeRelative(BlockCoord(x,y+1,z), chunkList)!=getBlockTypeRelative(BlockCoord(x,y,z), chunkList))
						pushFace(BlockCoord(x,y,z), BACK, chunkList);
					if (!getBlockTypeRelative(BlockCoord(x,y-1,z), chunkList)->IsOpaque() && getBlockTypeRelative(BlockCoord(x,y-1,z), chunkList)!=getBlockTypeRelative(BlockCoord(x,y,z), chunkList))
						pushFace(BlockCoord(x,y,z), FRONT, chunkList);
				}
			}
		}
	}
	IsGeneratedVertexData = true;
	ChunkActionMutex.unlock();
}

/*
void Chunk::NearbyChunkGeneration(Chunk& chunk)
{
	if (chunk.chunkCoord.z > chunkCoord.z)
		for (int x = 0; x < Chunk::WIDTH; x++)
			for (int y = 0; y < Chunk::WIDTH; y++)
				if (!chunk.GetBlockTypeAt(x, y, 0)->IsOpaque())
					pushFace(BlockCoord(x, y, Chunk::WIDTH - 1), TOP);
				else;
	if (chunk.chunkCoord.z < chunkCoord.z)
		for (int x = 0; x < Chunk::WIDTH; x++)
			for (int y = 0; y < Chunk::WIDTH; y++)
				if (!chunk.GetBlockTypeAt(x, y, Chunk::WIDTH - 1)->IsOpaque())
					pushFace(BlockCoord(x, y, Chunk::WIDTH - 1), BOTTOM);
				else;
	else if (chunk.chunkCoord.x > chunkCoord.x)
		for (int y = 0; y < Chunk::WIDTH; y++)
			for (int z = 0; z < Chunk::WIDTH; z++)
				if (!chunk.GetBlockTypeAt(0, y, z)->IsOpaque())
					pushFace(BlockCoord(Chunk::WIDTH - 1, y, z), LEFT);
				else;
	else if (chunk.chunkCoord.x < chunkCoord.x)
		for (int y = 0; y < Chunk::WIDTH; y++)
			for (int z = 0; z < Chunk::WIDTH; z++)
				if (!chunk.GetBlockTypeAt(Chunk::WIDTH - 1, y, z)->IsOpaque())
					pushFace(BlockCoord(0, y, z), RIGHT);
				else;
	else if (chunk.chunkCoord.y > chunkCoord.y)
		for (int x = 0; x < Chunk::WIDTH; x++)
			for (int z = 0; z < Chunk::WIDTH; z++)
				if (!chunk.GetBlockTypeAt(x, 0, z)->IsOpaque())
					pushFace(BlockCoord(x, Chunk::WIDTH - 1, z), FRONT);
				else;
	else if (chunk.chunkCoord.y < chunkCoord.y)
		for (int x = 0; x < Chunk::WIDTH; x++)
			for (int z = 0; z < Chunk::WIDTH; z++)
				if (!chunk.GetBlockTypeAt(x, Chunk::WIDTH - 1, z)->IsOpaque())
					pushFace(BlockCoord(x, 0, z), BACK);
				else;
}
*/
//NOTE: "coord" refers to the coord of the block within the chunk, not globally.  Not a usual usage.
void Chunk::pushFace(BlockCoord coord, FaceType type, Chunk** nearbyChunks)
{
	unsigned short currIdx = vertexData.size();
	float x = (float)(coord.x + chunkCoord.x*Chunk::WIDTH), y = (float)(coord.y + chunkCoord.y*Chunk::WIDTH), z = (float)(coord.z + chunkCoord.z*Chunk::WIDTH);
	float texId = GetBlockTypeAt(coord)->GetFaceTextureId(type);
	switch (type)
	{
		case TOP: //0 == 0.0, 0.0, 1.0
			vertexData.push_back(Vertex(x,			y,			z,			0.0f,			0.0f, texId, 0, 
				vertexAO(getBlockTypeRelative(coord + BlockCoord(-1,0,1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(0,-1,1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(-1,-1,1), nearbyChunks)->IsOpaque()))); //0 
			vertexData.push_back(Vertex(x + 1.0f,	y,			z,			0.0f,			1.0f, texId, 0,
				vertexAO(getBlockTypeRelative(coord + BlockCoord(1,0,1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(0,-1,1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(1,-1,1), nearbyChunks)->IsOpaque()))); //1
			vertexData.push_back(Vertex(x,			y + 1.0f,	z,			1.0f,			0.0f, texId, 0,
				vertexAO(getBlockTypeRelative(coord + BlockCoord(-1,0,1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(0,1,1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(-1,1,1), nearbyChunks)->IsOpaque()))); //2
			vertexData.push_back(Vertex(x + 1.0f,	y + 1.0f,	z,			1.0f,			1.0f, texId, 0,
				vertexAO(getBlockTypeRelative(coord + BlockCoord(1,0,1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(0,1,1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(1,1,1), nearbyChunks)->IsOpaque()))); //3
			break;																						  
		case BOTTOM: //1 == 0.0, 0.0, -1.0																  
			vertexData.push_back(Vertex(x,			y,			z - 1.0f,	0.0f,			0.0f, texId, 1, 
				vertexAO(getBlockTypeRelative(coord + BlockCoord(-1,0,-1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(0,-1,-1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(-1,-1,-1), nearbyChunks)->IsOpaque()))); //0 
			vertexData.push_back(Vertex(x,			y + 1.0f,	z - 1.0f,	0.0f,			1.0f, texId, 1,
				vertexAO(getBlockTypeRelative(coord + BlockCoord(1,0,-1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(0,-1,-1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(1,-1,-1), nearbyChunks)->IsOpaque()))); //1
			vertexData.push_back(Vertex(x + 1.0f,	y,			z - 1.0f,	1.0f,			0.0f, texId, 1,
				vertexAO(getBlockTypeRelative(coord + BlockCoord(-1,0,-1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(0,1,-1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(-1,1,-1), nearbyChunks)->IsOpaque()))); //2
			vertexData.push_back(Vertex(x + 1.0f,	y + 1.0f,	z - 1.0f,	1.0f,			1.0f, texId, 1,
				vertexAO(getBlockTypeRelative(coord + BlockCoord(1,0,-1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(0,1,-1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(1,1,-1), nearbyChunks)->IsOpaque()))); //3
			break;																						  
		case LEFT: //2 == 1.0, 0.0, 0.0																	  
			vertexData.push_back(Vertex(x + 1.0f,	y,			z - 1.0f,	0.0f,			0.0f, texId, 2,
				vertexAO(getBlockTypeRelative(coord + BlockCoord(1,-1,0), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(1,0,-1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(1,-1,-1), nearbyChunks)->IsOpaque()))); //0 
			vertexData.push_back(Vertex(x + 1.0f,	y + 1.0f,	z - 1.0f,	1.0f,			0.0f, texId, 2,
				vertexAO(getBlockTypeRelative(coord + BlockCoord(1,1,0), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(1,0,-1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(1,1,-1), nearbyChunks)->IsOpaque()))); //1
			vertexData.push_back(Vertex(x + 1.0f,	y,			z,			0.0f,			1.0f, texId, 2,
				vertexAO(getBlockTypeRelative(coord + BlockCoord(1,-1,0), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(1,0,1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(1,-1,1), nearbyChunks)->IsOpaque()))); //2
			vertexData.push_back(Vertex(x + 1.0f,	y + 1.0f,	z,			1.0f,			1.0f, texId, 2,
				vertexAO(getBlockTypeRelative(coord + BlockCoord(1,1,0), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(1,0,1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(1,1,1), nearbyChunks)->IsOpaque()))); //3
			break;																						  
		case RIGHT: //3 == -1.0, 0.0, 0.0																  
			vertexData.push_back(Vertex(x,			y,			z - 1.0f,	0.0f,			0.0f, texId, 3,
				vertexAO(getBlockTypeRelative(coord + BlockCoord(-1,-1,0), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(-1,0,-1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(-1,-1,-1), nearbyChunks)->IsOpaque()))); //0 
			vertexData.push_back(Vertex(x,			y,			z,			0.0f,			1.0f, texId, 3,
				vertexAO(getBlockTypeRelative(coord + BlockCoord(-1,-1,0), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(-1,0,1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(-1,-1,1), nearbyChunks)->IsOpaque()))); //1
			vertexData.push_back(Vertex(x,			y + 1.0f,	z - 1.0f,	1.0f,			0.0f, texId, 3,
				vertexAO(getBlockTypeRelative(coord + BlockCoord(-1,1,0), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(-1,0,-1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(-1,1,-1), nearbyChunks)->IsOpaque()))); //2
			vertexData.push_back(Vertex(x,			y + 1.0f,	z,			1.0f,			1.0f, texId, 3,
				vertexAO(getBlockTypeRelative(coord + BlockCoord(-1,1,0), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(-1,0,1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(-1,1,1), nearbyChunks)->IsOpaque()))); //3
			break;																						  
		case FRONT: //4 == 0.0, -1.0, 0.0																  
			vertexData.push_back(Vertex(x,			y,			z - 1.0f,	0.0f,			0.0f, texId, 4,
				vertexAO(getBlockTypeRelative(coord + BlockCoord(-1,-1,0), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(0,-1,-1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(-1,-1,-1), nearbyChunks)->IsOpaque()))); //0 
			vertexData.push_back(Vertex(x + 1.0f,	y,			z - 1.0f,	1.0f,			0.0f, texId, 4,
				vertexAO(getBlockTypeRelative(coord + BlockCoord(1,-1,0), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(0,-1,-1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(1,-1,-1), nearbyChunks)->IsOpaque()))); //1
			vertexData.push_back(Vertex(x,			y,			z,			0.0f,			1.0f, texId, 4,
				vertexAO(getBlockTypeRelative(coord + BlockCoord(-1,-1,0), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(0,-1,1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(-1,-1,1), nearbyChunks)->IsOpaque()))); //2
			vertexData.push_back(Vertex(x + 1.0f,	y,			z,			1.0f,			1.0f, texId, 4,
				vertexAO(getBlockTypeRelative(coord + BlockCoord(1,-1,0), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(0,-1,1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(1,-1,1), nearbyChunks)->IsOpaque()))); //3
			break;																						  
		case BACK: //5 == 0.0, 1.0, 0.0																	  
			vertexData.push_back(Vertex(x,			y + 1.0f,	z - 1.0f,	0.0f,			0.0f, texId, 5,
				vertexAO(getBlockTypeRelative(coord + BlockCoord(-1,1,0), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(0,1,-1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(-1,1,-1), nearbyChunks)->IsOpaque()))); //0 
			vertexData.push_back(Vertex(x,			y + 1.0f,	z,			0.0f,			1.0f, texId, 5,
				vertexAO(getBlockTypeRelative(coord + BlockCoord(-1,1,0), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(0,1,1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(-1,1,1), nearbyChunks)->IsOpaque()))); //1
			vertexData.push_back(Vertex(x + 1.0f,	y + 1.0f,	z - 1.0f,	1.0f,			0.0f, texId, 5,
				vertexAO(getBlockTypeRelative(coord + BlockCoord(1,1,0), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(0,1,-1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(1,1,-1), nearbyChunks)->IsOpaque()))); //2
			vertexData.push_back(Vertex(x + 1.0f,	y + 1.0f,	z,			1.0f,			1.0f, texId, 5,
				vertexAO(getBlockTypeRelative(coord + BlockCoord(1,1,0), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(0,1,1), nearbyChunks)->IsOpaque(), getBlockTypeRelative(coord + BlockCoord(1,1,1), nearbyChunks)->IsOpaque()))); //3
			break;
	}
	//triangle 1
	indexData.push_back(currIdx);
	indexData.push_back(currIdx + 1);
	indexData.push_back(currIdx + 2);
	//triangle 2
	indexData.push_back(currIdx + 3);
	indexData.push_back(currIdx + 2);
	indexData.push_back(currIdx + 1);
}

void Chunk::GenerateVBO()
{
	ChunkActionMutex.lock();
	if (indexData.size() > 0)
	{
		glGenBuffers(1, &vertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glGenBuffers(1, &indexBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexData.size(), &vertexData[0], GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * indexData.size(), &indexData[0], GL_STATIC_DRAW);
	}
	ChunkActionMutex.unlock();
	IsGeneratedVBO = true;
}

unsigned long Chunk::Draw()
{
	if (indexData.size()>0) 
	{
		GLuint64 timeElapsed = 0;
		GLuint query;
		GLint available = 0;

		//glGenQueries(1, &query);
	
		
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); //pos
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(float))); //uv/textureId
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6*sizeof(float)));
		glVertexAttribIPointer(3, 1, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)(7*sizeof(float))); //face enum
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
		//glBeginQuery(GL_TIME_ELAPSED, query);

		glDrawElements(GL_TRIANGLES, indexData.size(), GL_UNSIGNED_SHORT, (void*)0);
	
		//glEndQuery(GL_TIME_ELAPSED);

		//while (!available)
		//	glGetQueryObjectiv(query, GL_QUERY_RESULT_AVAILABLE, &available);

		//glGetQueryObjectui64v(query, GL_QUERY_RESULT, &timeElapsed);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		return timeElapsed;
	}
	else return 0;
}
