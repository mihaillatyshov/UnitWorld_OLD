#pragma once

#include "RigidBody.h"

struct CircleData
{
	float Radius;
};


class Circle : public RigidBody
{
public:
	Circle(const RigidBodyData &rbd, const CircleData &cd);
	
	void Draw() const override;
	TYPE GetType() override { return TYPE::TYPE_CIRCLE; }

	inline float GetRadius() { return m_Radius; }

protected:
	float m_Radius;
};