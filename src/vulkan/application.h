#pragma once

#include "window.h"

namespace eng {
	class Application {
	public:
		void run();

	private:
		Window window{ 1280, 720, "Vulkan Engine" };
	};
}