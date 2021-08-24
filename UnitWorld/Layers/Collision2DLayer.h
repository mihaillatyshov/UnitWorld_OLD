#pragma once

#include <vector>

#include "Layer.h"
#include "Physics/RigidBody.h"


class Collision2DLayer : public Layer
{
public:
	Collision2DLayer();
	~Collision2DLayer();
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;

	void OnImGuiRender() override;

protected:
	std::vector<RigidBody*> m_Objects;
	float DeltaTime;
};