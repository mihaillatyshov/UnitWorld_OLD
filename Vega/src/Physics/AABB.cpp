#include "AABB.h"

#include <glfw3.h>
#include <gl/glew.h>

AABB::AABB(const RigidBodyData &rbd, const AABBData &d)
	: RigidBody(rbd), m_HalfSize(d.Size / 2.0f)
{

}


void AABB::Draw() const
{
	glLineWidth(2);

	glBegin(GL_LINE_LOOP);
	glVertex2f(m_Position.x - m_HalfSize.x, m_Position.y - m_HalfSize.y);
	glVertex2f(m_Position.x + m_HalfSize.x, m_Position.y - m_HalfSize.y);
	glVertex2f(m_Position.x + m_HalfSize.x, m_Position.y + m_HalfSize.y);
	glVertex2f(m_Position.x - m_HalfSize.x, m_Position.y + m_HalfSize.y);
	glEnd();

	glLineWidth(1);

}