#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <shaderc/shaderc.hpp>     

#include <string>
namespace Application
{
	class Shader
	{
	public:
		Shader();
		~Shader();

		Shader& CreateShader(const VkDevice& device, const char* file, shaderc_shader_kind shaderKind,
			VkShaderStageFlagBits stage, const char* entryPoint);

		void Destroy(const VkDevice& device);

		inline const VkPipelineShaderStageCreateInfo& GetInfo() { return _shaderInfo; }

	private:
		VkPipelineShaderStageCreateInfo _shaderInfo;
		std::string						_code;

		std::string ReadFile(const std::string& file);
		VkShaderModule CreateShaderModule(VkDevice device, const std::vector<uint32_t>& code);
	};
}