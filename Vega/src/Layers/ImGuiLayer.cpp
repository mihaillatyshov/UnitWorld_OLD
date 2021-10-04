#include "ImGuiLayer.h"

#include "ImGui/ImGuiOpenGLRenderer.h"
#include "ImGui/ImGuiGLFWRenderer.h"
#include "Debug/ConsoleLog.h"

ImGuiLayer::ImGuiLayer()
	: m_Time(0.0f)
{

}


void ImGuiLayer::OnAttach()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	//io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	io.Fonts->AddFontDefault();
	ImGui_ImplGlfw_InitForOpenGL(Window::s_Window->GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
}


void ImGuiLayer::OnDetach()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}


void ImGuiLayer::Start()
{
	LM::ConsoleLog::SetColor(LM::ConsoleLog::ConsoleTxtColor::Cyan);
	ImGuiIO& io = ImGui::GetIO(); (void)io;
		
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
		
		
	static bool p_open;
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::SetNextWindowBgAlpha(0.0f);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &p_open, window_flags);
	ImGui::PopStyleVar(3);

	ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_KeepAliveOnly;
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	ImGui::End();
	LM::ConsoleLog::SetColor();
}


void ImGuiLayer::End()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}


void ImGuiLayer::OnImGuiRender()
{

		
	static bool show = true;
	if (show) 
		ImGui::ShowDemoWindow(&show);

	
	ChangeClearColor();


	static bool my_tool_active = true;
	if (my_tool_active)
	{
		if (!ImGui::Begin("TestMenu", &my_tool_active, ImGuiWindowFlags_MenuBar))
		{
			ImGui::End();
		}
		else
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
					if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
					if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
		
			if (ImGui::Button("MenuButton"))
			{
		
			}
			ImGui::End();
		}
	}


	

	
}


void ImGuiLayer::ChangeClearColor()
{
	if (ImGui::Begin("ChangeClearColor"))
	{
		ImGui::ColorEdit4("ClearColor", Window::s_Window->GetClearColor());
	}
	ImGui::End();
}