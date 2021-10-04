#include "Circle.h"

#include <glfw3.h>
#include <gl/glew.h>

Circle::Circle(const RigidBodyData &rbd, const CircleData &cd)
	: RigidBody(rbd), m_Radius(cd.Radius)
{

}


void Circle::Draw() const
{
	glLineWidth(2);
	glEnable(GL_LINE_SMOOTH);

	glBegin(GL_LINE_LOOP);
	for (float i = 0.0f; i < glm::radians(360.0f); i += glm::radians(360.0f) / 64.0f)
	{
		glVertex2f(m_Position.x + cos(i) * m_Radius, m_Position.y + sin(i) * m_Radius);
	}
	glEnd();

	glDisable(GL_LINE_SMOOTH);
	glLineWidth(1);

}