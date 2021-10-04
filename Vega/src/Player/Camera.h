#pragma once


#include <gtx/transform.hpp>

#include <cmath>


class Camera
{
public:
	Camera(const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec2 &angle = glm::vec2(0.0f, 0.0f));


	virtual glm::mat4 GetMaxtrix() const;
	inline void move(const glm::vec3 &vector) { m_Position += vector; }

	virtual void addRotation(const glm::vec2 &XY);

	void addPositionF(const float speed);
	void addPositionS(const float speed);
	void addPositionU(const float speed);

	//void setPosition(const glm::vec3 &pos);
	void setAngle(const glm::vec2 &angle) { m_Angle = angle; }
	//void setForward();


	const glm::vec3 &getPosition() const { return m_Position; }

	// Renurn Value X angle, Y angle 
	const glm::vec2 &getAngle() const { return m_Angle; }


	void SetPosition(const glm::vec3 &position) { m_Position = position; }
	
	void SetAngle(const glm::vec2& angle) { m_Angle = angle; addRotation(glm::vec2(0.0f)); }


	const glm::vec3 &getForward() const { return m_Forward; }

	//void drawLine();

	virtual void calculateCameraUp() const;
	virtual void calculateCameraEye() const;
	virtual void calculateCameraCenter() const;

	inline const glm::vec3 &getEye() { return m_CameraEye; }
private:
	void CorrectRotation();

protected:
	glm::vec3 m_Position;
	glm::vec3 m_Forward;
	static inline glm::vec3 m_WorldUp = glm::vec3(0, -1, 0);

	mutable glm::vec3 m_CameraEye;
	mutable glm::vec3 m_CameraCenter;
	mutable glm::vec3 m_CameraUp;

	glm::vec2 m_Angle;
};