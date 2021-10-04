#include "Texture.h"
#include <iostream>

#include <Debug/ConsoleLog.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture2D::Texture2D(std::string_view filename, unsigned mask)
{
	m_Filename = filename;
#if 1
	int channels = 0;
	stbi_set_flip_vertically_on_load(true);
	stbi_uc* data = stbi_load(filename.data(), &m_Width, &m_Height, &channels, 0);

		
	if (!data)
	{
		LOGE("Can't load texture: ", filename);
	}
	//glGenTextures(1, &m_TID);
	//glBindTexture(GL_TEXTURE_2D, m_TID);
	//glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, m_Width, m_Height);
	//	
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, data);
	////GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]));
	//glBindTexture(GL_TEXTURE_2D, 0);
	//LOGI(channels);
	GLint inFormat = channels == 4 ? GL_RGBA : GL_RGB;
	GLint outFormat = GL_RGBA;
	if (channels == 3)
		m_Width -= 1;

	glGenTextures(1, &m_TID);
	glBindTexture(GL_TEXTURE_2D, m_TID);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, outFormat, m_Width, m_Height, 0, inFormat, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
#else

	//unsigned mask = MASK::NONE;
	m_TID = load(mask);

#endif
}


Texture2D::~Texture2D()
{
	glDeleteTextures(1, &m_TID);
}


void Texture2D::Bind(uint32_t slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_TID);
	//glBindTextureUnit(slot, m_TID);
	
}
