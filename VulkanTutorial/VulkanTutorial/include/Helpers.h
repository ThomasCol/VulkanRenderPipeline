#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>
#include "Context.h"#

uint32_t FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
void CreateImage(Context context, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
	VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
VkImageView CreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
bool HasStencilComponent(VkFormat format);
void TransitionImageLayout(Context context, VkImage image,
	VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
