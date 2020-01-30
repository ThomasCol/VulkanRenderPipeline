#pragma once

#include <vulkan/vulkan.h>

#include <vector>

class CommandPool;
class Context;

class CommandBuffer
{
public:
	CommandBuffer() = default;
	~CommandBuffer() = default;

	CommandBuffer& BeginOneTime(Context context);
	void EndOneTime(Context context);

	VkCommandBuffer& Get() { return _buffer; }

private:
	VkCommandBuffer _buffer;

};