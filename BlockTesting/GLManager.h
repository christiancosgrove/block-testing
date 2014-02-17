#pragma once
#include <map>
#include <GL/glew.h>
#include <SFML\OpenGL.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>


class GLManager
{
public:

	GLManager(void);
	~GLManager(void);
	std::map<std::string, GLuint> Shaders;
	GLuint Program;
	GLuint PreprocessedFBO;
	void CompileShader(std::string, std::string, GLenum);
	void InitProgram();
	//static GLuint LoadBMP(std::string imagePath);
	static GLuint LoadTGA(std::string imagePath, bool);
	void SetupRenderingFBO();
private:
};

