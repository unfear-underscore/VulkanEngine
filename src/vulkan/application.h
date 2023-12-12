#pragma once

#include "window.h"
#include "pipeline.h"
#include <string>

namespace eng {
	const std::string ROOT_DIR_PATH = "../../../src/";

	class Application {
	public:
		void run();

	private:
		Window window{ 1280, 720, "Vulkan Engine" };
		Pipeline pipeline{ ROOT_DIR_PATH + "shaders/compiled/simple.vert.spv", ROOT_DIR_PATH + "shaders/compiled/simple.frag.spv" };
	};
}