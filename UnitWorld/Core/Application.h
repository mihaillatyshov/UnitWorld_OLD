#pragma once

#include "Core/Window.h"
#include "Core/LayerStack.h"
#include "Core/Texture.h"
#include "Core/Material.h"
#include "Renderers/Renderer.h"
#include "Layers/ImGuiLayer.h"
#include "Manager/Manager.h"

class Application
{
public:
	Application(std::string_view appName);
	virtual ~Application();

	inline Window* GetWindow() { return m_Window; }
	
	void PushLayer(Layer* layer);
	void PushOverlay(Layer* overlay);

	Manager<Texture>& GetTextureManager() { return m_TextureManager; }
	Manager<Material>& GetMaterialManager() { return m_MaterialManager; }

	void Run();
	
protected:
	Window* m_Window;

	Renderer* m_Renderer;

	Manager<Texture> m_TextureManager;
	Manager<Material> m_MaterialManager;

	ImGuiLayer* m_ImGuiLayer;
	LayerStack m_LayerStack;

	float m_LastFrameTime = 0.0f;

	inline static Application* m_Instance = nullptr;
};