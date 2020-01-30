#include "Buffer.h"

#include <stdexcept>

Buffer& Buffer::CreateBuffer(Context context, VkDeviceSize size, VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(context.device, &bufferInfo, nullptr, &_buffer) != VK_SUCCESS)
		throw std::runtime_error("failed to create buffer!");

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(context.device, _buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(context.physicalDevice, memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(context.device, &allocInfo, nullptr, &_bufferMemory) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate buffer memory!");

	vkBindBufferMemory(context.device, _buffer, _bufferMemory, 0);
}

uint32_t Buffer::FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			return i;
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

void Buffer::MapMemory(VkDevice device, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void* data)
{
	void* dataToBind;
	vkMapMemory(device, _bufferMemory, offset, size, flags, &dataToBind);
	memcpy(dataToBind, data, (size_t)size);
	vkUnmapMemory(device, _bufferMemory);
}

void Buffer::CopyBuffer(Context context, Buffer& dstBuffer, VkDeviceSize size)
{
	CommandBuffer commandBuffer;
	commandBuffer.BeginOneTime(context);

	VkBufferCopy copyRegion = {};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer.Get(), _buffer, dstBuffer._buffer, 1, &copyRegion);

	commandBuffer.EndOneTime(context);
}

void Buffer::Destroy(VkDevice device)
{
	vkDestroyBuffer(device, _buffer, nullptr);
	vkFreeMemory(device, _bufferMemory, nullptr);
}