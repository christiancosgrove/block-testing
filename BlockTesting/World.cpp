#include "World.h"
#include "MainGame.h"


World::World() : 
	player(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT), temp(0),
	sunAngle(4.73f),
	worldDisposed(false),
	chunksMustBeGenerated(true)
{
	std::thread cthread(&World::handleChunks, this);
	cthread.detach();
	player.Camera.Position = glm::vec3(-0.5f, 0.0f, 11.f);
}

void World::handleChunks()
{
	/*{
		std::unique_lock<std::mutex> lk(chunkMutex);
		chunkCV.wait(lk, [this] { return chunksMustBeGenerated || worldDisposed; });
	}*/
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	if (worldDisposed) return;
	/*chunkMapMutex.lock();
		//if (chunkActions.front().chunk!=nullptr) std::unique_lock<std::mutex> lk(chunkActions.front().chunk->ChunkActionMutex);
		switch (chunkActions.front().type)
		{
		case GENERATE_BLOCK_DATA:
			chunks[chunkActions.front().coord]->GenerateChunkBlockData();
			break;
		case GENERATE_VERTEX_DATA:
			chunks[chunkActions.front().coord]->GenerateVertexData();
			break;
		case NEW_CHUNK:
			addChunk(chunkActions.front().coord);
			break;
		case WORLD_DISPOSED:
			return;
		}
		chunkMapMutex.unlock();*/
	generateChunks();
	handleChunks();
}

void World::PrepareToDraw(GLManager glManager)
{
	textureManager.AddTexture(glManager, "C:\\Users\\Christian\\Desktop\\terrain.tga", "color_texture", true);
	textureManager.AddTexture(glManager, "C:\\Users\\Christian\\Desktop\\normal2.tga", "normal_texture", true);
	matrixLocation = glGetUniformLocation(glManager.Program, "viewMatrix");
	lightDirLocation = glGetUniformLocation(glManager.Program, "lightDir");
	cameraPosLocation = glGetUniformLocation(glManager.Program, "cameraPos");
	glUniform1f(glGetUniformLocation(glManager.Program, "render_distance"), Chunk::WIDTH * RENDER_DISTANCE);
}

void World::addChunk(ChunkCoord coord)
{
	chunks[coord] = new Chunk(coord, chunks);
	chunks[coord]->GenerateChunkBlockData();
}


void World::generateChunks()
{
	int xi = -(int)(player.Camera.Position.x / Chunk::WIDTH + 0.5f);
	int yi = -(int)(player.Camera.Position.y / Chunk::WIDTH + 0.5f);
	chunkMapMutex.lock();
	for (int i = -RENDER_DISTANCE; i<RENDER_DISTANCE; i++) for (int j = -RENDER_DISTANCE; j<RENDER_DISTANCE; j++)
	{
		int x = xi+i;
		int y = yi+j;
		if (chunks.find(ChunkCoord(x,y))==chunks.end())
		{
			addChunk(ChunkCoord(x,y,0));
			addChunk(ChunkCoord(x,y,1));
			addChunk(ChunkCoord(x,y,2));
			addChunk(ChunkCoord(x,y,3));
			addChunk(ChunkCoord(x,y,4));
			addChunk(ChunkCoord(x,y,5));
		}
	}
	auto it = chunks.begin();
	while (it!=chunks.end())
	{
		if (it->second->chunkCoord.x < xi - RENDER_DISTANCE || it->second->chunkCoord.x > xi + RENDER_DISTANCE || it->second->chunkCoord.y < yi - RENDER_DISTANCE || it->second->chunkCoord.y > yi + RENDER_DISTANCE)
		{
			it->second->~Chunk();
			it = chunks.erase(it);
		}
		else 
		{
			if (!it->second->IsGeneratedVertexData && it->second->IsGeneratedBlocks)
			{
				it->second->GenerateVertexData();
			}
			++it;
		}
	}
	chunkMapMutex.unlock();
	chunksMustBeGenerated = false;
}


World::~World(void)
{
	worldDisposed = true;
	chunkCV.notify_one();
	chunkMapMutex.lock();
	for (auto& it : chunks)
	{
		it.second->~Chunk();
	}
	chunkMapMutex.unlock();
}

void World::Draw()
{
	glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, glm::value_ptr(player.Camera.GetTransformMatrix()));
	glm::vec3 lightDir = (sunAngle<6.28f) ? glm::vec3(std::cosf(sunAngle), 0.0f, std::sinf(sunAngle)) : glm::vec3();
	glUniform3f(lightDirLocation, std::cosf(sunAngle), 0.0f, std::sinf(sunAngle));
	glUniform3f(cameraPosLocation, player.Camera.Position.x, player.Camera.Position.y, player.Camera.Position.z);

	sunAngle+=0.01f/MainGame::FRAME_LIMIT;
	if (sunAngle>9.42f) sunAngle = 3.141592653579f;
	

	glm::vec3 viewDirection = player.Camera.GetViewDirection();
	
	chunkMapMutex.lock();
	for (auto& it : chunks)
	{
		glm::vec3 disp = glm::vec3(it.second->chunkCoord.x, it.second->chunkCoord.y, it.second->chunkCoord.z)*(float)Chunk::WIDTH + player.Camera.Position;
		if (!it.second->IsGeneratedVBO)
		{
			it.second->GenerateVBO();
		}
		if (it.second->IsGeneratedVertexData && it.second->IsGeneratedVBO && glm::dot(disp, viewDirection) <= Chunk::WIDTH)
		{
			std::unique_lock<std::mutex> lk(it.second->ChunkActionMutex);
			it.second->Draw();
		}
	}
	chunkMapMutex.unlock();

	player.Update();
	if ((player.Camera.Position - player.LastChunkLoadPos).length() > 1)
	{
		chunksMustBeGenerated=true;
		player.LastChunkLoadPos = player.Camera.Position;
		//chunkCV.notify_one();
	}

}


void World::ResizedWindow(int windowWidth, int windowHeight)
{
	player.Camera.ResizedWindow(windowWidth, windowHeight);
}

/*
void World::bindVertexbuffer()
{
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertexData.size(), &vertexData[0], GL_STATIC_DRAW);
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indexData.size(), &indexData[0], GL_STATIC_DRAW);
}
*/