#pragma once

#include <gl/glew.h>
#include <glfw3.h>

#include <string>

#define GLCall(x) x

class Window
{
protected:
	bool Init();
public:
	Window(std::string_view title, int width, int height);
	~Window();

	bool Closed() const { return glfwWindowShouldClose(m_Window) == 1; }
	void Clear();
	void Update();

	inline GLFWwindow *GetWindow() { return m_Window; }
	inline float* GetClearColor() { return m_ClearColor; }

	friend void glfw_error_callback(int error, const char* description);

	// Resize window
	friend void window_resize(GLFWwindow *window, int width, int height);

public:
	static inline Window* s_Window;
protected:
	std::string m_Title;
	int m_Width;
	int m_Height;
	GLFWwindow *m_Window;
	float m_ClearColor[4] {0.0f, 0.0f, 0.0f, 1.0f};
};