#include "Camera.h"

Camera::Camera(const glm::vec3 &position, const glm::vec2 &angle)
	: m_Position(position), m_Forward(glm::vec3(0, 0, -1)), m_Angle(angle)
{
}

void Camera::addRotation(const glm::vec2 &XY)
{
	m_Angle += XY;
	CorrectRotation();

	m_Forward = glm::rotate(glm::radians(m_Angle.x), glm::vec3(0.0f, -1.0f, 0.0f)) *
				glm::rotate(glm::radians(m_Angle.y), glm::vec3(1.0f,  0.0f, 0.0f)) * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
	m_Forward = glm::normalize(m_Forward);

}

void Camera::addPositionF(const float speed)
{
	m_Position += m_Forward * speed;
}

void Camera::addPositionS(const float speed)
{
	m_Position += glm::normalize(glm::cross(m_Forward, m_CameraUp)) * speed;
}

void Camera::addPositionU(const float speed)
{
	m_Position.y += speed;
}

glm::mat4 Camera::GetMaxtrix() const
{ 
	calculateCameraUp();
	calculateCameraEye();
	calculateCameraCenter();

	return glm::lookAt(m_CameraEye, m_CameraCenter, m_CameraUp);
}
	
void Camera::calculateCameraUp() const
{
	m_CameraUp = glm::rotate(glm::radians(m_Angle.x), glm::vec3(0.0f, -1.0f, 0.0f)) *
					glm::rotate(glm::radians(m_Angle.y), glm::vec3(1.0f,  0.0f, 0.0f)) * glm::vec4(m_WorldUp, 0.0f);
	m_CameraUp = glm::normalize(m_CameraUp);
}


void Camera::calculateCameraEye() const
{
	m_CameraEye = m_Position;
}

void Camera::calculateCameraCenter() const
{
	m_CameraCenter = m_Position + m_Forward;
}

void Camera::CorrectRotation()
{
	m_Angle.x = fmodf(m_Angle.x, 360.0f);
	//m_Angle.x -= int(m_Angle.x / 360.0f) * 360.0f;
	//if (m_Angle.x < 0)
	//	m_Angle.x += 360.0f;
		
	if (m_Angle.y >  89.0f)
		m_Angle.y =  89.0f;
	if (m_Angle.y < -89.0f)
		m_Angle.y = -89.0f;
}