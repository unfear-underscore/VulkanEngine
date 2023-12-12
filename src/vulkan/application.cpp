#include "application.h"

void eng::Application::run()
{
	while (!window.shouldClose()) {
		glfwPollEvents();
	}
}
