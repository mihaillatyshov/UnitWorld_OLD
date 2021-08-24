#pragma once

#include <GL/glew.h>

#include "Layer.h"
#include "Core/Shader.h"

#include "TestBoneAnim/mesh.h"
#include "TestBoneAnim/SceneAnimator.h"


class TestBoneAnimLayer : public Layer
{
protected:
public:
	TestBoneAnimLayer();
	~TestBoneAnimLayer();

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;

	void OnImGuiRender() override;

protected:
	Mesh m_Mesh;
	Shader* m_MeshSahder;

	glm::vec3 m_MeshPosition;
	glm::vec2 m_MeshRotation;
	glm::vec3 m_MeshScale;

	int m_UsedAnimation = 0;

	bool m_UseLight = true;
	glm::vec3 m_LightPosition;
	float m_Time;
	float m_NowTime;
	bool m_Stop;
	bool m_Loop;

	AssimpView::SceneAnimator *m_SceneAnimator = nullptr;
};