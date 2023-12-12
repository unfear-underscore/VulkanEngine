#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <iostream>

#include "device.h"

namespace eng {

	struct PipelineConfig {

	};

	class Pipeline {
	public:
		Pipeline(Device& device, const PipelineConfig& config, const std::string& vertFilePath, const std::string& fragFilePath);
		~Pipeline() {};

		Pipeline(const Pipeline&) = delete;
		void operator=(const Pipeline&) = delete;

		static PipelineConfig createDefaultConfig(uint32_t width, uint32_t height);

	private:
		Device& device;
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;

		static std::vector<char> readFile(const std::string& filePath);
		void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfig& config);
		void createShaderModule(const std::vector<char>& shader, VkShaderModule* shaderModule);
	};
}