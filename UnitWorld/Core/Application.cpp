#include "Application.h"

#include <iostream>
#include "Layers/ImGuiLayer.h"
#include "Renderers/RealBatchRenderer.h"
#include "Timer.h"


#include "Layers/ImGuiLayer.h"
#include "Layers/TestBoneAnimLayer.h"
#include "Layers/TrianglesLayer.h"
#include "Layers/RealBatchRendererLayer.h"
#include "Layers/BatchRendererLayer.h"
#include "Layers/SharedLayer.h"
#include "Layers/Collision2DLayer.h"
#include "Layers/GeometryShaderLayer.h"
#include "Debug/ConsoleLog.h"

Application::Application(std::string_view appName)
{
	LOG_INIT();

	LOGI("st1 ", "st2 ", 32, ' ', 3.0f, ' ', "dsaq");

	if (m_Instance)
	{
		LOGI("Application already exists!!!");
		return;
	}
	m_Instance = this;
	
	m_Window = new Window(appName, 800, 600);

	m_Renderer = new RealBatchRenderer();
	m_Renderer->OnAttach();
	Renderer::Set(m_Renderer);

	m_ImGuiLayer = new ImGuiLayer();
	PushOverlay(m_ImGuiLayer);

	//PushLayer(new TestBoneAnimLayer);							// NEED TO DELETE!!!
	//PushLayer(new TrianglesLayer);							// NEED TO DELETE!!!
	//PushLayer(new LM::SharedLayer);								// NEED TO DELETE!!!
	//PushLayer(new RealBatchRendererLayer);						// NEED TO DELETE!!!
	//PushLayer(new LM::BatchRendererLayer);						// NEED TO DELETE!!!
	PushLayer(new LM::GeometryShaderLayer);						// NEED TO DELETE!!!
	//PushLayer(new Collision2DLayer);							// NEED TO DELETE!!!
}


Application::~Application()
{
	m_Renderer->OnDetach();
	//delete m_Renderer;
	//delete m_Window;
}


void Application::PushLayer(Layer* layer)
{
	m_LayerStack.PushLayer(layer);
	layer->OnAttach();
}
	

void Application::PushOverlay(Layer* overlay)
{
	m_LayerStack.PushOverlay(overlay);
	overlay->OnAttach();
}


void Application::Run()
{
	while (!m_Window->Closed()) // Add in Application class this data
	{
		//PROFILE_SCOPE("Game Loop");
		Timer timer;
		m_Window->Clear();

		for (Layer* layer : m_LayerStack)
			layer->OnUpdate();


		m_ImGuiLayer->Start();
		for (Layer* layer : m_LayerStack)
			layer->OnImGuiRender();
		m_ImGuiLayer->End();

		m_Window->Update();
	}
}



#if 0

class TestApp
{
	void Update()
	{		
		// Some code
		// ...

		if (MouseButtonClicked)
		{
			AddToIntermediateBuffer(new obj);

		}
	
		// Some code
		// ...



		SendDataToRendererIntermediateBuffer();
	}
};

#endif