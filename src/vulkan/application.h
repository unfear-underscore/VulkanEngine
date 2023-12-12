#pragma once

#include "window.h"
#include "pipeline.h"
#include "device.h"

#include <string>

namespace eng {
	const std::string ROOT_DIR_PATH = "../../../src/";

	class Application {
	public:
		void run();

	private:
		static constexpr int WIDTH = 1280;
		static constexpr int HEIGHT = 720;

		Window window{ WIDTH, HEIGHT, "Vulkan Engine" };
		Device device{ window };
		Pipeline pipeline{ device, Pipeline::createDefaultConfig(WIDTH, HEIGHT),  ROOT_DIR_PATH + "shaders/compiled/simple.vert.spv", ROOT_DIR_PATH + "shaders/compiled/simple.frag.spv" };
	};
}