#include "Window.h"
#include <iostream>
#include "Debug/ConsoleLog.h"

void glfw_error_callback(int error, const char* description);
void window_resize(GLFWwindow *window, int width, int height);

Window::Window(std::string_view title, int width, int height)
	: m_Title(title), m_Width(width), m_Height(height)
{
	if (!Init())
	{
		glfwTerminate();
	}
	s_Window = this;
}

Window::~Window()
{
	delete m_Window;
}

bool Window::Init()
{
	if (!glfwInit()) {
		LOGE("Failed to initialize GLFW!");
		return false;
	}

	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);

	if (!m_Window) {
		LOGE("Failed to create window!");
		return false;
	}
	glfwMakeContextCurrent(m_Window);
	glfwSetWindowUserPointer(m_Window, this);

	glfwSetErrorCallback(glfw_error_callback);
	glfwSetFramebufferSizeCallback(m_Window, window_resize);
	glfwSetWindowSizeCallback(m_Window, window_resize);
	//glfwSetKeyCallback(m_Window, key_callback);
	//glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
	//glfwSetCursorPosCallback(m_Window, cursor_position_callback);
	//glfwSetScrollCallback(m_Window, scroll_callback);
	//glfwSetCharCallback(m_Window, character_input);
	//glfwSetDropCallback(m_Window, droping_paths);
	glfwSwapInterval(0);

	if (glewInit() != GLEW_OK) {
		LOGE("Could not initialise GLEW!");
		return false;
	}

	GLCall(glEnable(GL_BLEND)); 
	//glEnable(GL_ALPHA_TEST);
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


	LOGI("OpenGL version: ", glGetString(GL_VERSION));
	LOGI("OpenGL vendor: ", glGetString(GL_VENDOR));
	LOGI("OpenGL renderer: ", glGetString(GL_RENDERER));
	//LOGI("OpenGL extensions: ", glGetString(GL_EXTENSIONS));
	LOGI("OpenGL shading language version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));

	GLint texture_units;
	GLCall(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_units));
	LOGI("Number of textures can be acessed by fragment shader: ", texture_units);
	GLCall(glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &texture_units));
	LOGI("Max texture units: ", texture_units);

	//system("pause");

	return true;
}

void Window::Clear()
{
	glfwMakeContextCurrent(m_Window);
	GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void Window::Update()
{
	glfwPollEvents();
	glfwSwapBuffers(m_Window);
}


void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void window_resize(GLFWwindow *window, int width, int height) 
{
	GLCall(glViewport(0, 0, width, height));
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->m_Width = width;
	win->m_Height = height;
}

