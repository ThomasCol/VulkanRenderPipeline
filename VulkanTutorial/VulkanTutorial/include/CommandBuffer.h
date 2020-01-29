#pragma once

#include <vulkan/vulkan.h>

#include <vector>

class CommandPool;

class CommandBuffer
{
public:
	CommandBuffer() = default;
	~CommandBuffer() = default;

	CommandBuffer& BeginOneTime(VkDevice device, CommandPool commandPool);
	void EndOneTime(VkDevice device, CommandPool commandPool, VkQueue graphicsQueue);

	VkCommandBuffer& Get() { return _buffer; }

private:
	VkCommandBuffer _buffer;

};