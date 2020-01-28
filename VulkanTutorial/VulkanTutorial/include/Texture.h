#pragma once

#include "Buffer.h"

class Texture
{
public:
	Texture() = default;
	~Texture() = default;

	void Load(const char* file);
	void CreateTexture(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicQueue);
	void TransitionImageLayout(VkDevice device, VkCommandPool commandPool, VkQueue graphicQueue, VkImage image,
		VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void CopyBufferToImage(VkDevice device, VkCommandPool commandPool, VkQueue graphicQueue, Buffer buffer, VkImage image, uint32_t width, uint32_t height);
	void CreateTextureImageView(VkDevice device);
	bool HasStencilComponent(VkFormat format);
	void CreateTextureSampler(VkDevice device);

	void Destroy(VkDevice device);

	inline VkImageView& GetView() { return _textureImageView; }
	inline VkSampler& GetSampler() { return _textureSampler; }

private:
	VkImage							_textureImage;
	VkDeviceMemory					_textureImageMemory;
	VkImageView						_textureImageView;
	VkSampler						_textureSampler;
	unsigned char*					_pixels;
	int								_texWidth;
	int								_texHeight;
};
