#pragma once

#include <glm.hpp>

class Camera;

class Player
{
public:
	Player(Camera* camera);
	virtual void OnUpdate();

	inline Camera* GetCamera() { return m_Camera; }


	inline static Player* Get() { return s_Player; }
protected:
	inline static Player* s_Player = nullptr;
	glm::vec3 m_Position;
	float m_Speed = 4.0f;
	Camera* m_Camera;
};