#include "GLManager.h"


GLManager::GLManager(void)
{
}

GLManager::~GLManager(void)
{
}

void GLManager::SetupRenderingFBO()
{

}

void GLManager::CompileShader(std::string path, std::string name, GLenum type)
{
	GLuint id = glCreateShader(type);

	std::string code;
	std::fstream shaderStream(path.c_str(), std::ios::in);
	if (shaderStream.is_open())
	{
		std::string line = "";
		while (std::getline(shaderStream, line))
		{
			code+="\n" + line;
		}
		shaderStream.close();
	}

	GLint result = GL_FALSE;
	int infoLogLength;

	const char* sourceString = code.c_str();
	glShaderSource(id, 1, &sourceString,NULL);
	glCompileShader(id);

	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> errorMessage(infoLogLength);
	glGetShaderInfoLog(id, infoLogLength, NULL, &errorMessage[0]);
	fprintf(stdout, "%s\n", &errorMessage[0]);
	Shaders[name] = id;
}

void GLManager::InitProgram()
{
	fprintf(stdout, "Linking program...\n");
	Program = glCreateProgram();
	
	for (std::map<std::string, GLuint>::iterator it = Shaders.begin(); it != Shaders.end(); ++it)
	{
		glAttachShader(Program, it->second);
	}
	glLinkProgram(Program);

	GLint result;
	int infoLogLength;
	glGetProgramiv(Program, GL_LINK_STATUS, &result);
	glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> errorMessage(std::max(infoLogLength, int(1)));
	glGetProgramInfoLog(Program, infoLogLength, NULL, &errorMessage[0]);
	fprintf(stdout, "%s\n", &errorMessage[0]);
	glUseProgram(Program);
}

//deprecated
/*
GLuint GLManager::LoadBMP(std::string imagePath)
{
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize;

	unsigned char* data;
	FILE* file = fopen(imagePath.c_str(), "rb");
	if (!file)
	{
		printf("Bitmap image could not be opened.\n");
		return 0;
	}
	if (fread(header, 1, 54, file)!=54)
	{
		printf("Not a correct BMP file...\n");
		return false;
	}
	if (header[0]!='B' || header[1]!='M') 
	{
		printf("Not a correct BMP file...\n");
		return 0;
	}
	dataPos		 = *(int*)&(header[0x0A]);
	imageSize	 = *(int*)&(header[0x22]);
	width		 = *(int*)&(header[0x12]);
	height		 = *(int*)&(header[0x16]);

	if (imageSize==0)
		imageSize = width*height*3;
	if (dataPos==0)
		dataPos = 54;

	data = new unsigned char[imageSize];
	fread(data, 1, imageSize, file);
	fclose(file);

	GLuint textureId;
	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	delete[] data;
	return textureId;
}
*/
GLuint GLManager::LoadTGA(std::string imagePath, bool mipmap)
{
	unsigned char header[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	unsigned char bpp = 32;
	unsigned char id = 8;
	unsigned short width;
	unsigned short height;
	unsigned char *pPixels = NULL;
	FILE* fp = NULL;

	fp = fopen(imagePath.c_str(),"r");

	fread(header,sizeof(unsigned char),12,fp);
	fread(&width,sizeof(unsigned short),1,fp);
	fread(&height,sizeof(unsigned short),1,fp);
	fread(&bpp,sizeof(unsigned char),1,fp);
	fread(&id,sizeof(unsigned char),1,fp);

	pPixels = new unsigned char[width * height * 4];
	fread(pPixels, 4, width * height, fp);
	fclose(fp);
	
	GLuint textureId;
	glGenTextures(1, &textureId);
	

	glBindTexture(GL_TEXTURE_2D_ARRAY, textureId);
	//use texture_2d_arrays because of mippmapping and linear filtering -- no filtering/mipmapping between tiles in texture atlas

	glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipmap ? 8 : 1, GL_RGBA8, width, width, height/width);
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, width, height/width, GL_BGRA, GL_UNSIGNED_BYTE, pPixels);
	//glTexImage3D(GL_TEXTURE_2D_ARRAY, GL_RGBA8, 0, width, width, height/width, 0, GL_BGRA, GL_UNSIGNED_BYTE, pPixels);
	if (mipmap) glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_WRAP);
	//glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
	delete[] pPixels;
	return textureId;
}