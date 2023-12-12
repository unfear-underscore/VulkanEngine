#include "window.h"

eng::Window::Window(int width, int height, std::string windowName) : width{ width }, height{ height }, windowName{ windowName }
{
	initWindow();
}

eng::Window::~Window()
{
	glfwDestroyWindow(window);

	glfwTerminate();
}

void eng::Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
{
	if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create window surface");
	}
}

void eng::Window::initWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Don't make OpenGL context, since we aren't using OpenGL
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
}