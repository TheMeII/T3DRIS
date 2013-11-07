#pragma once

#include <string>
#include <iostream>

#ifdef WIN32
#include <Windows.h>
#endif

#include <GL\glew.h>
#include <magick\Magick++.h>

class Texture
{
public:
	Texture(GLenum TextureTarget, const std::string& FileName); //when loading texture, specify target(GL_TEXTURE_2D) and file name.
	bool Load(); //load texture
	void Bind(GLenum TextureUnit); //bind texture to object

private:
	std::string m_fileName;
	GLenum m_textureTarget;
	GLuint m_textureObj;
	Magick::Image* m_pImage;
	Magick::Blob m_blob; //blob is (Binary Large Object) is useful mechanism for storing an encoded image in memory in way it can be used by external programs
};


