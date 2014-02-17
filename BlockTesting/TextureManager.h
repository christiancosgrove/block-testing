#pragma once
#include "GLManager.h"
#include <unordered_map>


class TextureManager
{
public:
	TextureManager(void);
	~TextureManager(void);
	std::vector<GLuint> Textures;
	void AddTexture(GLManager&, std::string, std::string, bool);
};

