#pragma once

#include "Context.h"
#include "Vertex.h"
#include "Texture.h"
#include "CommandPool.h"

#define MODEL_PATH "Media/chalet.obj"

class Mesh
{
public:
	Mesh() = default;
	~Mesh() = default;

	Mesh& LoadMesh(const char* modelFile, const char* textureFile = "");

	void CreateVertexBuffer(Context context);
	void CreateBuffers(Context context);
	void CreateIndexBuffer(Context context);

	void Destroy(VkDevice device);

	inline VkPipelineVertexInputStateCreateInfo& GetInfo() { return _info; };

	inline const VkBuffer& GetVertexBuffer() { return _vertexBuffer.GetBuffer(); }
	inline const VkBuffer& GetIndexBuffer() { return _indexBuffer.GetBuffer(); }
	inline const uint32_t& GetIndexSize() { return static_cast<uint32_t>(_indices.size()); }
	inline VkImageView& GetTextureView() { return _texture.GetView(); }
	inline VkSampler& GetTextureSampler() { return _texture.GetSampler(); }

private:
	std::vector<Vertex>				_vertices;
	std::vector<uint32_t>			_indices;
	Buffer							_vertexBuffer;
	Buffer							_indexBuffer;
	Texture							_texture;

	VkPipelineVertexInputStateCreateInfo	_info;
	VkVertexInputBindingDescription			_bindingDescriptor;
	std::array<VkVertexInputAttributeDescription, 3>	_attributeDescriptions;

};