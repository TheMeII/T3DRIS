#include "texture.h"

Texture::Texture(GLenum TextureTarget, const std::string& FileName)
{
	m_textureTarget = TextureTarget;
	m_fileName = FileName;
	m_pImage = NULL; //image loaded later
}

bool Texture::Load()
{
	try
	{
		m_pImage = new Magick::Image(m_fileName); //load image from file
		m_pImage->write(&m_blob, "RGBA"); //write image to blob and set image color channels: RED, GREEN, BLUE, ALPHA
	}

	catch(Magick::Error& Error) //use librarys error method to detect possible error
	{
		std::cout << "Error loading texture '" << m_fileName << ": " << Error.what() << std::endl;
		return false; //false if not succesful
	}
	//use glew to generate texture and bind it
	glGenTextures(1, &m_textureObj); //generates specified number of textures, and places handels of those in GLuint array  pointer (m_textureObj)

	glBindTexture(m_textureTarget, m_textureObj); //Tell openGL what texture object to oparate. It tells OpenGL the texture object we refer to in 
												//all the following texture related calls, until a new texture object is bound. Texture target is also specified.

	glTexImage2D(m_textureTarget, 0, GL_RGBA, m_pImage->columns(), m_pImage->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_blob.data()); //Used to load main part of texture object, taht is texture data itself
		//Params: 1: TestureTarget, 2: LOD(level-of-detail, bigger, the lower the resolutuion), 3: internal format that openGL stores the texture, 4: Widht of texture (Magick's image has function for this and next)
		// 5: Height of image ///LAST THREE SPECIFY SOURCE OF DATA// 6: Define chanels used in blob, 7: Type describes core data type that each chanel have, 8: Memory address of actual data that has been created before

	//Specify filters to be used for magnification and minification, used to properly match object, that it is applied to (size of object and texture usualy are diffrent from each other)
	glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true; //true if successful
}

void Texture::Bind(GLenum TextureUnit) //with this we bind our created texture to some object, 
	//that needs to be told before draw call, so it's sampled in fragment shader. When called, this will stay active until new call of this
{
	glActiveTexture(TextureUnit); //make given texture unit enum(GL_TEXTURE0, GL_TEXTURE1, etc.) active 
	glBindTexture(m_textureTarget, m_textureObj); //and bind texture object to it.
}


