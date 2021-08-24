#pragma once

#include <imgui.h>
#include <Core/Window.h>

#include "Layer.h"

class ImGuiLayer : public Layer
{
protected:
	void ChangeClearColor();
public:
	static void Start();
	static void End();

	ImGuiLayer();

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate() override { }

	void OnImGuiRender() override;
protected:
	float m_Time;
};