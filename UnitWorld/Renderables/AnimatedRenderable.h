#pragma once

#include "Renderable.h"

class AnimatedRenderable : Renderable
{
public:
	AnimatedRenderable(std::string_view FileName);

	virtual void Submit(Renderer* renderer) override;
protected:
};