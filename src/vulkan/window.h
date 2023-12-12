#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace eng {
	class Window {
	public:
		Window(int width, int height, std::string windowName);
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		bool shouldClose() { return glfwWindowShouldClose(window); }
	private:
		void initWindow();

		GLFWwindow* window;
		const int width;
		const int height;
		const std::string windowName;
	};
}