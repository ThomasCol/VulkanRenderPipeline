#include "Shader.h"

#include <string>
#include <fstream>

namespace Application
{
	Shader::Shader() :
		_shaderInfo{}
	{}

	Shader::~Shader()
	{
	}

	std::string Shader::ReadFile(const std::string& filename)
	{
		std::string str = filename;
		std::ifstream file(str, std::ios::ate);
		if (!file.is_open())
			throw std::runtime_error("failed to open file!");

		size_t fileSize = (size_t)file.tellg();
		std::string buffer(fileSize, ' ');

		file.seekg(0);
		file.read(&buffer[0], fileSize);
		file.close();

		buffer[fileSize] = '\0';

		return buffer;
	}

	Shader& Shader::CreateShader(const VkDevice& device, const char* file, shaderc_shader_kind shaderKind,
		VkShaderStageFlagBits stage, const char* entryPoint)
	{
		_code = ReadFile(file);

		shaderc::Compiler compiler;

		shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(_code, shaderKind, file);
		shaderc_compilation_status status = result.GetCompilationStatus();
		if (status != shaderc_compilation_status_success)
			throw std::runtime_error(result.GetErrorMessage());

		std::vector<uint32_t> shaderSPV;
		shaderSPV.assign(result.cbegin(), result.cend());

		VkShaderModule shaderModule = CreateShaderModule(device, shaderSPV);
		_shaderInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		_shaderInfo.stage = stage;
		_shaderInfo.module = shaderModule;
		_shaderInfo.pName = entryPoint;
	}

	VkShaderModule Shader::CreateShaderModule(VkDevice device, const std::vector<uint32_t>& code)
	{
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = 4 * code.size();
		createInfo.pCode = code.data();

		VkShaderModule shaderModule;

		if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
			throw std::runtime_error("failed to create shader module!");

		return shaderModule;
	}

	void Shader::Destroy(const VkDevice& device)
	{
		vkDestroyShaderModule(device, _shaderInfo.module, nullptr);
	}
}