#include "pipeline.h"

eng::Pipeline::Pipeline(Device& device, const PipelineConfig& config, const std::string& vertFilePath, const std::string& fragFilePath) : device{ device }
{
	createGraphicsPipeline(vertFilePath, fragFilePath, config);
}

eng::PipelineConfig eng::Pipeline::createDefaultConfig(uint32_t width, uint32_t height)
{
	PipelineConfig config{};
	return config;
}

std::vector<char> eng::Pipeline::readFile(const std::string& filePath)
{
	std::ifstream file{ filePath, std::ios::ate | std::ios::binary };

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file: " + filePath);
	}

	size_t fileSize = static_cast<size_t>(file.tellg());

	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}

void eng::Pipeline::createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfig& config)
{
	auto vertFile = readFile(vertFilePath);
	auto fragFile = readFile(fragFilePath);

	std::cout << vertFile.size() << std::endl << fragFile.size() << std::endl;
}

void eng::Pipeline::createShaderModule(const std::vector<char>& shader, VkShaderModule* shaderModule)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = shader.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(shader.data());

	if (vkCreateShaderModule(device.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create shader module");
	}
}
