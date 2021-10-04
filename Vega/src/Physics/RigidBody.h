#pragma once

#include <glm.hpp>

struct RigidBodyData
{
	glm::vec2 Position;
	glm::vec2 Velocity;
	float Mass;
};


class RigidBody
{
public:
	RigidBody(const RigidBodyData &rbd)
		: m_Position(rbd.Position), m_Velocity(rbd.Velocity)
	{
		m_InvMass = rbd.Mass == 0.0f ? 0.0f : 1.0f / rbd.Mass;
	}

	virtual ~RigidBody() = default;

	enum class TYPE
	{
		TYPE_CIRCLE = 0,
		TYPE_AABB,
		TYPE_OBB,

		TYPE_COUNT
	};

	virtual TYPE GetType() = 0;

	inline void SetVelocity(const glm::vec2 &velocity) { m_Velocity = velocity; }
	inline void AddVelocity(const glm::vec2 &vec) { m_Velocity += vec; }

	inline const glm::vec2 &GetPosition() { return m_Position; }
	inline const glm::vec2 &GetVelocity() { return m_Velocity; }
	inline float GetInvMass() { return m_InvMass; }

	virtual void Draw() const { }

	virtual void Integrate(float dt) { m_Position += m_Velocity * dt; }

protected:
	glm::vec2 m_Position;
	glm::vec2 m_Velocity;
	float m_InvMass;

};