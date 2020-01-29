#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define TEXTURE_PATH "Media/chalet.jpg"
#include <stdexcept>
#include "Buffer.h"

#include "Helpers.h"

void Texture::Load(const char* file)
{
	if (file == "")
		return;
	int texChannels;
	_pixels = stbi_load(file, &_texWidth, &_texHeight, &texChannels, STBI_rgb_alpha);

	if (!_pixels)
		throw std::runtime_error("failed to load texture image!");
}

void Texture::CreateTexture(VkDevice device, VkPhysicalDevice physicalDevice,
		CommandPool commandPool, VkQueue graphicQueue)
{
	if (_pixels == nullptr)
		return;

	VkDeviceSize imageSize = _texWidth * _texHeight * 4;
	Buffer stagingBuffer;
	stagingBuffer.CreateBuffer(device, physicalDevice, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	stagingBuffer.MapMemory(device, 0, imageSize, 0, _pixels);

	stbi_image_free(_pixels);

	CreateImage(device, physicalDevice, _texWidth, _texHeight, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		_textureImage, _textureImageMemory);

	TransitionImageLayout(device, commandPool, graphicQueue, _textureImage,
			VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(device, commandPool, graphicQueue, stagingBuffer, _textureImage, static_cast<uint32_t>(_texWidth), static_cast<uint32_t>(_texHeight));
	TransitionImageLayout(device, commandPool, graphicQueue, _textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	stagingBuffer.Destroy(device);

	CreateTextureImageView(device);
	CreateTextureSampler(device);
}

void Texture::CopyBufferToImage(VkDevice device, CommandPool commandPool, VkQueue graphicQueue, Buffer buffer, VkImage image, uint32_t width, uint32_t height)
{
	CommandBuffer commandBuffer;
	commandBuffer.BeginOneTime(device, commandPool);

	VkBufferImageCopy region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = {
		width,
		height,
		1
	};

	vkCmdCopyBufferToImage(
		commandBuffer.Get(),
		buffer.GetBuffer(),
		image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		1,
		&region
	);

	commandBuffer.EndOneTime(device, commandPool, graphicQueue);
}

void Texture::CreateTextureImageView(VkDevice device)
{
	_textureImageView = CreateImageView(device, _textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT);
}

void Texture::CreateTextureSampler(VkDevice device)
{
	VkSamplerCreateInfo samplerInfo = {};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = 16;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	if (vkCreateSampler(device, &samplerInfo, nullptr, &_textureSampler) != VK_SUCCESS)
		throw std::runtime_error("failed to create texture sampler!");
}

void Texture::Destroy(VkDevice device)
{
	vkDestroySampler(device, _textureSampler, nullptr);
	vkDestroyImageView(device, _textureImageView, nullptr);

	vkDestroyImage(device, _textureImage, nullptr);
	vkFreeMemory(device, _textureImageMemory, nullptr);
}