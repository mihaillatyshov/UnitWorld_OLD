#include "ThirdPersonCamera.h"

ThirdPersonCamera::ThirdPersonCamera(const glm::vec3 &position, const glm::vec2 &angle, float range)
	: Camera(position, angle), m_Range(range)
{
		
}

glm::mat4 ThirdPersonCamera::GetMaxtrix() const
{ 
	calculateCameraUp();
	calculateCameraEye();
	calculateCameraCenter();

	return glm::lookAt(m_CameraEye, m_CameraCenter, m_CameraUp);
}

void ThirdPersonCamera::calculateCameraEye() const
{
	m_CameraEye = m_Position - m_Forward * m_Range;
}

void ThirdPersonCamera::calculateCameraCenter() const
{
	m_CameraCenter = m_Position;
}