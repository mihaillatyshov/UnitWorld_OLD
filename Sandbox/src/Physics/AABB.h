#pragma once

#include "RigidBody.h"


struct AABBData
{
	glm::vec2 Size;
};


class AABB : public RigidBody
{
	public:
	AABB(const RigidBodyData &rbd, const AABBData &d);
	
	void Draw() const override;
	TYPE GetType() override { return TYPE::TYPE_AABB; }

	inline const glm::vec2 &GetHalfSize() { return m_HalfSize; }

protected:
	glm::vec2 m_HalfSize;
};