#pragma once

#include <vector>
#include <glm.hpp>
#include <memory>

#include "Player/Camera.h"

class Renderable;
class AnimatedRenderable;

class Projection
{
public:
	Projection(const glm::mat4& transform = glm::mat4(1.0f)) : m_Transform(transform) { }
	virtual glm::mat4 GetTransform() const { return m_Transform; }
protected:
	glm::mat4 m_Transform;
};

class Lights
{
public:
	Lights()
	{
		s_Lights = this;
	}
	virtual glm::vec3 GetDirection() const { return glm::rotate(glm::radians(Angle.x), glm::vec3(0.0f, -1.0f, 0.0f)) *
													glm::rotate(glm::radians(Angle.y), glm::vec3(-1.0f,  0.0f, 0.0f)) * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);}
	
	inline static Lights* Get() { return s_Lights; }

	virtual const glm::vec3& GetAmbient() const { return Ambient; }
	virtual const glm::vec3& GetDiffuse() const { return Diffuse; }
	virtual const glm::vec3& GetSpecular() const { return Specular; }

public:
	glm::vec2 Angle = glm::vec2(0.0f);

	glm::vec3 Ambient = glm::vec3(0.1f);
	glm::vec3 Diffuse = glm::vec3(0.7f);
	glm::vec3 Specular = glm::vec3(1.0f);
protected:
	inline static Lights* s_Lights = nullptr;
};

class Renderer
{
public:
	Renderer() = default;
	virtual ~Renderer() = default;

	friend Renderable;

	virtual void OnAttach() = 0;
	virtual void OnDetach() = 0;
	virtual void OnUpdate() = 0;
	virtual void BeginScene(Camera *camera, const Projection &projection, Lights *lights) = 0;
	virtual void Flush() = 0;

	//inline static Renderer* GetRenderer() { return s_Renderer; }
	virtual void PushTransform(glm::mat4 *transform, bool override = false) = 0;
	virtual void PopTransform() = 0;

	inline static Renderer* Get() { return s_Renderer; }
	inline static void Set(Renderer* renderer) { s_Renderer = renderer; }
	virtual void Submit(std::shared_ptr<Renderable> renderable) { };  // FIX
protected:
	virtual void Submit(Renderable* renderable) = 0;
	virtual void Submit(AnimatedRenderable* renderable) = 0; 
protected:
	inline static Renderer* s_Renderer = nullptr;
};