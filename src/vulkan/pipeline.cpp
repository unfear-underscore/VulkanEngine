#include "pipeline.h"

eng::Pipeline::Pipeline(Device& device, const PipelineConfig& config, const std::string& vertFilePath, const std::string& fragFilePath) : device{ device }
{
	createGraphicsPipeline(vertFilePath, fragFilePath, config);
}

eng::Pipeline::~Pipeline()
{
	vkDestroyShaderModule(device.device(), vertShaderModule, nullptr);
	vkDestroyShaderModule(device.device(), fragShaderModule, nullptr);

	vkDestroyPipeline(device.device(), graphicsPipeline, nullptr);
}

eng::PipelineConfig eng::Pipeline::createDefaultConfig(uint32_t width, uint32_t height)
{
	PipelineConfig config{};

	config.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	config.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	config.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	config.viewport.x = 0.0f;
	config.viewport.y = 0.0f;
	config.viewport.width = static_cast<float>(width);
	config.viewport.height = static_cast<float>(height);
	config.viewport.minDepth = 0.0f;
	config.viewport.maxDepth = 1.0f;

	config.scissor.offset = { 0, 0 };
	config.scissor.extent = { width, height };

	config.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	config.viewportInfo.viewportCount = 1;
	config.viewportInfo.pViewports = &config.viewport;
	config.viewportInfo.scissorCount = 1;
	config.viewportInfo.pScissors = &config.scissor;

	config.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	config.rasterizationInfo.depthClampEnable = VK_FALSE;
	config.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
	config.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
	config.rasterizationInfo.lineWidth = 1.0f;
	config.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
	config.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	config.rasterizationInfo.depthBiasEnable = VK_FALSE;
	config.rasterizationInfo.depthBiasConstantFactor = 0.0f;
	config.rasterizationInfo.depthBiasClamp = 0.0f;
	config.rasterizationInfo.depthBiasSlopeFactor = 0.0f;

	config.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	config.multisampleInfo.sampleShadingEnable = VK_FALSE;
	config.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	config.multisampleInfo.minSampleShading = 1.0f;
	config.multisampleInfo.pSampleMask = nullptr;
	config.multisampleInfo.alphaToCoverageEnable = VK_FALSE;
	config.multisampleInfo.alphaToOneEnable = VK_FALSE;

	config.colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT;
	config.colorBlendAttachment.blendEnable = VK_FALSE;
	config.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	config.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	config.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	config.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	config.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	config.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	config.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	config.colorBlendInfo.logicOpEnable = VK_FALSE;
	config.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
	config.colorBlendInfo.attachmentCount = 1;
	config.colorBlendInfo.pAttachments = &config.colorBlendAttachment;
	config.colorBlendInfo.blendConstants[0] = 0.0f;
	config.colorBlendInfo.blendConstants[1] = 0.0f;
	config.colorBlendInfo.blendConstants[2] = 0.0f;
	config.colorBlendInfo.blendConstants[3] = 0.0f;

	config.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	config.depthStencilInfo.depthTestEnable = VK_TRUE;
	config.depthStencilInfo.depthWriteEnable = VK_TRUE;
	config.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
	config.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
	config.depthStencilInfo.minDepthBounds = 0.0f;
	config.depthStencilInfo.maxDepthBounds = 1.0f;
	config.depthStencilInfo.stencilTestEnable = VK_FALSE;
	config.depthStencilInfo.front = {};
	config.depthStencilInfo.back = {};

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
	assert(config.pipelineLayout != VK_NULL_HANDLE && "Cannot create graphics pipeline: No pipelineLayout");
	assert(config.renderPass != VK_NULL_HANDLE && "Cannot create graphics pipeline: No renderPass");

	auto vertFile = readFile(vertFilePath);
	auto fragFile = readFile(fragFilePath);

	createShaderModule(vertFile, &vertShaderModule);
	createShaderModule(fragFile, &fragShaderModule);

	VkPipelineShaderStageCreateInfo shaderStages[2];
	// Vertex
	shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	shaderStages[0].module = vertShaderModule;
	shaderStages[0].pName = "main"; // entry function name
	shaderStages[0].flags = 0;
	shaderStages[0].pNext = nullptr;
	shaderStages[0].pSpecializationInfo = nullptr;

	// Fragment
	shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStages[2].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	shaderStages[3].module = fragShaderModule;
	shaderStages[4].pName = "main"; // entry function name
	shaderStages[5].flags = 0;
	shaderStages[6].pNext = nullptr;
	shaderStages[7].pSpecializationInfo = nullptr;

	VkPipelineVertexInputStateCreateInfo vertInputInfo{};
	vertInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertInputInfo.vertexAttributeDescriptionCount = 0;
	vertInputInfo.vertexBindingDescriptionCount = 0;
	vertInputInfo.pVertexAttributeDescriptions = nullptr;
	vertInputInfo.pVertexBindingDescriptions = nullptr;

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertInputInfo;
	pipelineInfo.pInputAssemblyState = &config.inputAssemblyInfo;
	pipelineInfo.pViewportState = &config.viewportInfo;
	pipelineInfo.pRasterizationState = &config.rasterizationInfo;
	pipelineInfo.pMultisampleState = &config.multisampleInfo;
	pipelineInfo.pColorBlendState = &config.colorBlendInfo;
	pipelineInfo.pDynamicState = nullptr;
	
	pipelineInfo.layout = config.pipelineLayout;
	pipelineInfo.renderPass = config.renderPass;
	pipelineInfo.subpass = config.subpass;

	pipelineInfo.basePipelineIndex = -1;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	if (vkCreateGraphicsPipelines(device.device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
		throw std::runtime_error("FGailed to create graphics pipeline");
	}
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
