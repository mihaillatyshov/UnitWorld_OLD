#pragma once

#include <string_view>
#include <GL/glew.h>


#define BIT(x) (1 << x)


class Texture
{
public:
	Texture() = default;
	virtual ~Texture() = default;

	virtual void Bind(uint32_t slot = 0) = 0;
	virtual uint32_t GetId() const = 0;
protected:
};

class Texture2D : public Texture
{
public:
	enum MASK : unsigned
	{
		NONE					= 0,

		SRGB					= 0,
		NO_SRGB					= BIT(0),

		ALPHA					= 0,
		NO_ALPHA				= BIT(1),

		LINEAR					= 0,
		NEAREST					= BIT(2),
		ALL_FILTER				= LINEAR | NEAREST,

		S_REPEAT				= 0,
		S_MIRRORED_REPEAT		= BIT(3),
		S_CLAMP_TO_EDGE			= BIT(4),
		//MIRROR_CLAMP_TO_EDGE ???
		S_CLAMP_TO_BORDER		= BIT(3) | BIT(4),
		S_ALL_WRAP				= S_REPEAT | S_MIRRORED_REPEAT | S_CLAMP_TO_EDGE | S_CLAMP_TO_BORDER,

		T_REPEAT				= 0,
		T_MIRRORED_REPEAT		= BIT(5),
		T_CLAMP_TO_EDGE			= BIT(6),
		T_CLAMP_TO_BORDER		= BIT(5) | BIT(6),
		T_ALL_WRAP				= T_REPEAT | T_MIRRORED_REPEAT | T_CLAMP_TO_EDGE | T_CLAMP_TO_BORDER,

		REPEAT					= S_REPEAT			| T_REPEAT,
		MIRRORED_REPEAT			= S_MIRRORED_REPEAT	| T_MIRRORED_REPEAT,
		CLAMP_TO_EDGE			= S_CLAMP_TO_EDGE	| T_CLAMP_TO_EDGE,
		CLAMP_TO_BORDER			= S_CLAMP_TO_BORDER	| T_CLAMP_TO_BORDER,
		ALL_WRAP				= S_ALL_WRAP		| T_ALL_WRAP,

		NO_MIPMAP				= 0,
		LINEAR_MIPMAP_LINEAR	= BIT(7),
		LINEAR_MIPMAP_NEAREST	= BIT(8),
		NEAREST_MIPMAP_LINEAR	= BIT(9),
		NEAREST_MIPMAP_NEAREST	= BIT(7) | BIT(8) | BIT(9),
		ALL_MIPMAP				= NO_MIPMAP | LINEAR_MIPMAP_LINEAR | LINEAR_MIPMAP_NEAREST | NEAREST_MIPMAP_LINEAR | NEAREST_MIPMAP_NEAREST
	};

public:
	Texture2D(std::string_view filename, unsigned mask = NONE);
	virtual ~Texture2D();

	void Bind(uint32_t slot = 0) override;
	uint32_t GetId() const override { return m_TID; }
	

protected:
	virtual GLuint load(unsigned mask);
		
	virtual int calcWrap(const unsigned mask, const unsigned mirrored, const unsigned edge, const unsigned border);
	virtual int calcMagFilter(bool NEAREST);
	virtual int calcMinFilter(const unsigned mask, const int magFilter);
	virtual int calcInputType(bool NO_SRGB);
	virtual int calcOutputType(bool NO_ALPHA);


protected:
	std::string m_Filename;
	int m_Width;
	int m_Height;

	uint32_t m_TID;
};