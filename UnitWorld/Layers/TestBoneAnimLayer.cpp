#include "TestBoneAnimLayer.h"

#define GLCall(x) x


#include <imgui.h>

#include <gtx/transform.hpp>
#include <iostream>
#include <glfw3.h>
#include "Debug/Instrumentor.h"


TestBoneAnimLayer::TestBoneAnimLayer() 
	: m_MeshPosition(0.0f, 0.0f, -80.0f), m_MeshRotation(180.0f, 90.0f), m_MeshScale(1.0f),
	m_LightPosition(0.0f), m_Time(0.0f)
{
	m_NowTime = glfwGetTime();
	m_Stop = true;
	m_Loop = false;
}

TestBoneAnimLayer::~TestBoneAnimLayer()
{
}


void TestBoneAnimLayer::OnAttach()
{
	m_MeshSahder = new Shader({ "./res/Shaders/Bone/Bone.vert", "./res/Shaders/Bone/Bone.frag" });

	m_Mesh.LoadMesh("./res/Models/BoneAnim5.fbx");
	//m_Mesh.LoadMesh("./res/Models/BoneAnim3.abc");
	//m_Mesh.LoadMesh("./res/Models/Col.dae");
	//m_Mesh.LoadMesh("./res/Models/boblampclean.md5mesh");

	m_SceneAnimator = new AssimpView::SceneAnimator(m_Mesh.GetScene(), m_UsedAnimation);



	GLint SizeBuffer;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &SizeBuffer);
	LOGI("GL_MAX_VERTEX_ATTRIBS: ", SizeBuffer);
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &SizeBuffer);
	LOGI("GL_MAX_UNIFORM_BLOCK_SIZE: ", SizeBuffer);
	glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &SizeBuffer);
	LOGI("GL_MAX_UNIFORM_LOCATIONS: ", SizeBuffer);
	
}


void TestBoneAnimLayer::OnDetach()
{
	delete m_SceneAnimator;
}


aiNode* FWM(aiNode *node) // TODO: Add mesh node pointer to every mesh (or only with bones)
{
	PROFILE_FUNCTION();
	if (node->mNumMeshes != 0)
	{
		return node;
	}
	for (int i = 0; i < node->mNumChildren; i++)
	{
		auto nd = FWM(node->mChildren[i]);
		if (nd != nullptr)
			return nd;
	}
	return nullptr;
}


void TestBoneAnimLayer::OnUpdate()
{

	m_MeshSahder->Enable();
	glm::mat4 Projection = glm::perspective(80.0f, 16.0f / 9.0f, 0.01f, 200.0f);
	glm::mat4 Model = glm::translate(m_MeshPosition) * ( 
					  glm::rotate(glm::radians(m_MeshRotation.x), glm::vec3(0.0f, -1.0f, 0.0f))  *
					  glm::rotate(glm::radians(m_MeshRotation.y), glm::vec3(1.0f,  0.0f, 0.0f))) *
					  glm::scale(m_MeshScale);

	glm::mat4 MVP = Projection * Model;

	m_MeshSahder->SetUniformMat4("uProjection", Projection);
	m_MeshSahder->SetUniformMat4("uModel", Model);

	m_MeshSahder->SetUniform3f("uLight", m_LightPosition);
	m_MeshSahder->SetUniform1f("useLight", m_UseLight ? 10 : -10);

	std::vector<Matrix4f> BoneTransform;
	if (m_UsedAnimation == m_Mesh.GetAnimationsCount())
	{
		for (int i = 0; i < 100; i++)
		{
			BoneTransform.emplace_back();
			BoneTransform[i].InitIdentity();
		}
	}
	else
	{
		//m_Mesh.Evaluate(m_Mesh.GetAnimation(m_UsedAnimation), m_Time, BoneTransform);
		//m_Mesh.BoneTransform(m_Time, BoneTransform, m_UsedAnimation);
		m_SceneAnimator->Calculate(m_Time);
		//LOGI(FWM(m_Mesh.GetScene()->mRootNode)->mNumMeshes);
		const std::vector<aiMatrix4x4>& boneMats = m_SceneAnimator->GetBoneMatrices(FWM(m_Mesh.GetScene()->mRootNode));
		//LOGI(boneMats.size());
		for (int i = 0; i < boneMats.size(); i++)
		{
			BoneTransform.push_back(boneMats[i]);
		}
	}


	for (int i = 0; i < BoneTransform.size(); i++)
	{
		std::string UniformName = "uBones[" + std::to_string(i) + "]";
		m_MeshSahder->SetUniformMat4(UniformName, BoneTransform[i]);
	}
	//LOGI(BoneTransform.size());



	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glDepthFunc(GL_LEQUAL));
	//GLCall(glEnable(GL_CULL_FACE));

	m_Mesh.Render();

	//GLCall(glDisable(GL_CULL_FACE));
	GLCall(glDisable(GL_DEPTH_TEST));
	

	float Time = glfwGetTime();
	if (!m_Stop) 
		m_Time += Time - m_NowTime;
	m_NowTime = Time;

	if (m_UsedAnimation != m_Mesh.GetAnimationsCount())
	{
		if (m_Time > m_Mesh.GetAnimation(m_UsedAnimation)->mDuration / m_Mesh.GetAnimation(m_UsedAnimation)->mTicksPerSecond)
		{
			if (m_Loop)
			{
				m_Time = fmodf(m_Time, m_Mesh.GetAnimation(m_UsedAnimation)->mDuration / m_Mesh.GetAnimation(m_UsedAnimation)->mTicksPerSecond);
			}
			else
			{
				m_Stop = true;
				m_Time = 0.0f;
			}
		}
	}
}

void TestBoneAnimLayer::OnImGuiRender()
{

	if (ImGui::Begin("TestBoneAnimLayer"))
	{
		ImGui::Checkbox("Use Light", &m_UseLight);
		ImGui::DragFloat3("Light", &m_LightPosition.x);
	}
	ImGui::End();

	if (ImGui::Begin("Mesh Bone"))
	{
		ImGui::DragFloat3("Mesh Position", &m_MeshPosition.x, 0.05f);
		ImGui::DragFloat2("Mesh Rotation", &m_MeshRotation.x, 0.01f);
		ImGui::DragFloat3("Mesh Scale", &m_MeshScale.x, 0.01f);
		ImGui::DragFloat("Bone Time", &m_Time, 0.01f, 0.0f, FLT_MAX);
		ImGui::Checkbox("Stop", &m_Stop);
		ImGui::Checkbox("Loop", &m_Loop);
		if (ImGui::Button("Reset"))
		{
			m_Time = 0.0f;
		}
	}
	ImGui::End();

	if (ImGui::Begin("Mesh Animations"))
	{
		if (m_Mesh.HasAnimations())
		{
			ImGui::Text("Mesh has %d Animations", m_Mesh.GetAnimationsCount());
			
			std::vector<const char*> AnimCombo;
			for (int i = 0; i < m_Mesh.GetAnimationsCount(); i++)
				AnimCombo.push_back(m_Mesh.GetAnimation(i)->mName.C_Str());
			AnimCombo.push_back("<NONE>");

			//for (int i = 0; i < m_Mesh.GetAnimationsCount(); i++)
			//	AnimCombo.push_back(std::to_string(i));
			//
			if (ImGui::Combo("Animations", &m_UsedAnimation, AnimCombo.data(), m_Mesh.GetAnimationsCount() + 1))
			{
				delete m_SceneAnimator;
				m_SceneAnimator = nullptr;
				if (m_UsedAnimation != m_Mesh.GetAnimationsCount())
					m_SceneAnimator = new AssimpView::SceneAnimator(m_Mesh.GetScene(), m_UsedAnimation);
			}

			if (m_UsedAnimation != m_Mesh.GetAnimationsCount())
			{
				ImGui::Text("Duration: %f", m_Mesh.GetAnimation(m_UsedAnimation)->mDuration);
				ImGui::Text("TPS: %f", m_Mesh.GetAnimation(m_UsedAnimation)->mTicksPerSecond);
				ImGui::Text("Animation time: %f", m_Mesh.GetAnimation(m_UsedAnimation)->mDuration / m_Mesh.GetAnimation(m_UsedAnimation)->mTicksPerSecond);
				ImGui::Text("Num Chanels: %u",m_Mesh.GetAnimation(m_UsedAnimation)->mNumChannels);
				ImGui::Text("Num Mesh Chanels: %u",m_Mesh.GetAnimation(m_UsedAnimation)->mNumMeshChannels);
				ImGui::Text("Num Morph Mesh Chanels: %u",m_Mesh.GetAnimation(m_UsedAnimation)->mNumMorphMeshChannels);


			}

		}
		else
		{
			ImGui::Text("Mesh has not Animations");
		}
	}
	ImGui::End();
}