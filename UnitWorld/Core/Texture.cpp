#include "Texture.h"
#include <iostream>


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <FreeImage.h>


Texture2D::Texture2D(std::string_view filename, unsigned mask)
{
	m_Filename = filename;
#if 0
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




template <class T> void INPLACESWAP(T& a, T& b) {
	a ^= b; b ^= a; a ^= b;
}

static BOOL SwapRedBlue32(FIBITMAP* dib) {
	if (FreeImage_GetImageType(dib) != FIT_BITMAP) {
		return FALSE;
	}

	const unsigned bytesperpixel = FreeImage_GetBPP(dib) / 8;
	if (bytesperpixel > 4 || bytesperpixel < 3) {
		return FALSE;
	}

	const unsigned height = FreeImage_GetHeight(dib);
	const unsigned pitch = FreeImage_GetPitch(dib);
	const unsigned lineSize = FreeImage_GetLine(dib);

	BYTE* line = FreeImage_GetBits(dib);
	for (unsigned y = 0; y < height; ++y, line += pitch) {
		for (BYTE* pixel = line; pixel < line + lineSize; pixel += bytesperpixel) {
			INPLACESWAP(pixel[0], pixel[2]);
		}
	}

	return TRUE;
}

static BYTE* load_image(const char *filename, int* width, int* height)
	{
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		FIBITMAP *dib = nullptr;

		fif = FreeImage_GetFileType(filename, 0);
		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(filename);
		if (fif == FIF_UNKNOWN)
			return nullptr;

		if (FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, filename);
		if (!dib)
			return nullptr;

		dib = FreeImage_ConvertToRGBA16(dib);
		dib = FreeImage_ConvertTo32Bits(dib);
		SwapRedBlue32(dib);

		BYTE* pixels = FreeImage_GetBits(dib);
		*width = FreeImage_GetWidth(dib);
		*height = FreeImage_GetHeight(dib);
		int bits = FreeImage_GetBPP(dib);

		//dib->data
		//RGBQUAD 
		//RGBQUAD test;
		//FreeImage_SwapColors(dib, &test, &test, true);
		//
		//for (int i = 0; i < *width * *height; i+=4)
		//	FreeImage_SwapPaletteIndices(dib, (BYTE*)(0 + i), (BYTE*)(2 + i));
		//
		//for (int i = 0; i < *width * *height * 4; i += 4)
		//{
		//	BYTE buf = pixels[i + 0];
		//	pixels[i + 0] = pixels[i + 2];
		//	pixels[i + 2] = buf;
		//}


		//if (textureInfo)
		//{
		//	textureInfo->BitsPerPixel = FreeImage_GetBPP(dib);
		//	textureInfo->Color = FreeImage_GetColorType(dib);
		//	textureInfo->DataSize = *width * *height * (bits / 8);
		//}

		int size = *width * *height * (bits / 8);
		BYTE* result = new BYTE[size];
		memcpy(result, pixels, size);
		FreeImage_Unload(dib);

		return result;
	}

GLuint Texture2D::load(unsigned mask)
{
	BYTE* pixels = load_image(m_Filename.c_str(), &m_Width, &m_Height);
		
	GLuint result;
	glGenTextures(1, &result);
	glBindTexture(GL_TEXTURE_2D, result);
		
	int inputType	= calcInputType(mask & MASK::NO_SRGB);
	int outType		= calcOutputType(mask & MASK::NO_ALPHA);
	int magFilter	= calcMagFilter(mask & MASK::ALL_FILTER);
	int minFilter	= calcMinFilter(mask & MASK::ALL_MIPMAP, magFilter);
	int clampS		= calcWrap(mask & MASK::S_ALL_WRAP, MASK::S_CLAMP_TO_EDGE, MASK::S_CLAMP_TO_BORDER, MASK::S_MIRRORED_REPEAT);
	int clampT		= calcWrap(mask & MASK::T_ALL_WRAP, MASK::T_CLAMP_TO_EDGE, MASK::T_CLAMP_TO_BORDER, MASK::T_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clampS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clampT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);


	glTexImage2D(GL_TEXTURE_2D, 0, inputType, m_Width, m_Height, 0, outType, GL_UNSIGNED_BYTE, pixels);
		
	if (magFilter != minFilter)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	delete[] pixels;
	return result;
}


int Texture2D::calcWrap(const unsigned mask, const unsigned mirrored, const unsigned edge, const unsigned border)
	{
		if (mask == mirrored)
			return GL_MIRRORED_REPEAT;
		if(mask == edge)
			return GL_CLAMP_TO_EDGE;
		if (mask == border)
			return GL_CLAMP_TO_BORDER;

		return GL_REPEAT;
	}

	int Texture2D::calcInputType(bool NO_SRGB)
	{
		return NO_SRGB ? GL_RGBA : GL_SRGB_ALPHA;
	}

	int Texture2D::calcOutputType(bool NO_ALPHA)
	{
		return NO_ALPHA ? GL_RGB : GL_RGBA;
	}

	int Texture2D::calcMagFilter(bool NEAREST)
	{
		return NEAREST ? GL_NEAREST : GL_LINEAR;
	}

	int Texture2D::calcMinFilter(const unsigned mask, const int magFilter)
	{
		if (mask == MASK::LINEAR_MIPMAP_LINEAR)
			return GL_LINEAR_MIPMAP_LINEAR;
		if (mask == MASK::LINEAR_MIPMAP_NEAREST)
			return GL_LINEAR_MIPMAP_NEAREST;
		if (mask == MASK::NEAREST_MIPMAP_LINEAR)
			return GL_NEAREST_MIPMAP_LINEAR;
		if (mask == MASK::NEAREST_MIPMAP_NEAREST)
			return GL_NEAREST_MIPMAP_NEAREST;

		return magFilter;

	}