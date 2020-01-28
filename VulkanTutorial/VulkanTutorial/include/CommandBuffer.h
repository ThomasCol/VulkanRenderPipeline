#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class CommandBuffer
{
public:
	CommandBuffer();
	~CommandBuffer();

	void BeginSingleTimeCommands();
	void EndSingleTimeCommand();

private:
	VkCommandBuffer _buffer;

};