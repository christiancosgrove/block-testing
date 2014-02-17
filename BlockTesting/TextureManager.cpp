#include "TextureManager.h"


TextureManager::TextureManager(void)
{
}


TextureManager::~TextureManager(void)
{
	for (int i = 0; i<Textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		glDisable(GL_TEXTURE_2D_ARRAY);
	}
}

void TextureManager::AddTexture(GLManager& glManager, std::string path, std::string name, bool mipmap)
{
	int currId = Textures.size();
	glActiveTexture(GL_TEXTURE0 + currId);
	Textures.push_back(GLManager::LoadTGA(path, mipmap));
	glUniform1i(glGetUniformLocation(glManager.Program, name.c_str()), currId);
	glBindTexture(GL_TEXTURE_2D_ARRAY, Textures[currId]);
}


