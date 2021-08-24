#pragma once

#include "Camera.h"

class ThirdPersonCamera : public Camera
{
public:
	ThirdPersonCamera(const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec2 &angle = glm::vec2(0.0f, 0.0f), float range = 4.0f);

	virtual void calculateCameraEye() const override;
	virtual void calculateCameraCenter() const override;
	

	virtual glm::mat4 GetMaxtrix() const;
protected:
	float m_Range;
};