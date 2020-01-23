#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <optional>

#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include <chrono>
#include <array>

#define WIDTH 800
#define HEIGHT 600
#define MAX_FRAMES_IN_FLIGHT 2

#define MODEL_PATH "Media/chalet.obj"
#define TEXTURE_PATH "Media/chalet.jpg"
 

struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;


	static VkVertexInputBindingDescription GetBindingDescription();

	static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions();

	bool operator==(const Vertex& other) const
	{
		return pos == other.pos && color == other.color && texCoord == other.texCoord;
	}
};

namespace std
{
	template<> struct hash<Vertex>
	{
		size_t operator()(Vertex const& vertex) const
		{
			return ((hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}

struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};


namespace Application
{
	class TriangleApp
	{
	private:
		GLFWwindow*						_window;
		VkInstance						_instance;
		VkDebugUtilsMessengerEXT		_debugMessenger;
		VkPhysicalDevice				_physicalDevice { VK_NULL_HANDLE };
		VkDevice 						_device;
		VkSurfaceKHR 					_surface;
		VkQueue 						_graphicsQueue;
		VkQueue 						_presentQueue;
		VkSwapchainKHR					_swapChain;
		std::vector<VkImage>			_swapChainImages;
		VkFormat						_swapChainImageFormat;
		VkExtent2D						_swapChainExtent;
		std::vector<VkImageView>		_swapChainImageViews;
		VkRenderPass					_renderPass;
		VkDescriptorSetLayout			_descriptorSetLayout;
		VkPipelineLayout				_pipelineLayout;
		VkPipeline						_graphicsPipeline;
		VkCommandPool					_commandPool;
		std::vector<VkCommandBuffer>	_commandBuffers;
		std::vector<VkSemaphore>		_imageAvailableSemaphores;
		std::vector<VkSemaphore>		_renderFinishedSemaphores;
		std::vector<VkFence>			_inFlightFences;
		std::vector<VkFence>			_imagesInFlight;
		size_t							_currentFrame = 0;
		std::vector<VkFramebuffer>		_swapChainFramebuffers;
		std::vector<Vertex>				_vertices;
		std::vector<uint32_t>			_indices;
		VkBuffer						_vertexBuffer;
		VkDeviceMemory					_vertexBufferMemory;
		VkBuffer						_indexBuffer;
		VkDeviceMemory					_indexBufferMemory;
		std::vector<VkBuffer>			_uniformBuffers;
		std::vector<VkDeviceMemory>		_uniformBuffersMemory;
		VkDescriptorPool				_descriptorPool;
		std::vector<VkDescriptorSet>	_descriptorSets;
		VkImage							_textureImage;
		VkDeviceMemory					_textureImageMemory;
		VkImageView						_textureImageView;
		VkSampler						_textureSampler;
		VkImage							_depthImage;
		VkDeviceMemory					_depthImageMemory;
		VkImageView						_depthImageView;

		const std::vector<const char*> _validationLayers {
			"VK_LAYER_KHRONOS_validation"
		};

		const std::vector<const char*> _deviceExtensions {
    		VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		struct QueueFamilyIndices
		{
    		std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;

    		bool isComplete()
			{
        		return graphicsFamily.has_value();
   			}
		};

		struct SwapChainSupportDetails
		{
    		VkSurfaceCapabilitiesKHR capabilities;
    		std::vector<VkSurfaceFormatKHR> formats;
    		std::vector<VkPresentModeKHR> presentModes;
		};

#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif

		void InitWindow();
		void InitVulkan();
		void CreateInstance();
		void CreateLogicalDevice();
		void CreateSurface();
		void CreateSwapChain();
		void CreateImageViews();
		void CreateRenderPass();
		void CreateDescriptorSetLayout();
		void CreateGraphicsPipeline();
		VkShaderModule CreateShaderModule(const std::vector<char>& code);
		void CreateFramebuffers();
		void CreateCommandPool();
		VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkFormat FindDepthFormat();
		bool HasStencilComponent(VkFormat format);
		void CreateDepthResources();
		void CreateTextureImage();
		void CreateTextureSampler();
		void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
				VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
		void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		void CreateTextureImageView();
		VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
		void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void LoadModel();
		void CreateVertexBuffer();
		void CreateIndexBuffer();
		void CreateUniformBuffers();
		void CreateDescriptorPool();
		void CreateDescriptorSets();
		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		void CreateCommandBuffers();
		VkCommandBuffer BeginSingleTimeCommands();
		void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
		void CreateSyncObjects();
		void SetupDebugMessenger();
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void CheckExtension();
		std::vector<const char*> GetRequiredEtensions();
		bool CheckValidationLayerSupport();
		void PickPhysicalDevice();
		bool IsDeviceSuitable(VkPhysicalDevice device);
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
		int  RateDeviceSuitability(VkPhysicalDevice device);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		void MainLoop();
		void DrawFrame();
		void UpdateUniformBuffer(uint32_t currentImage);
		void Cleanup();
		void CleanupSwapChain();

		void RecreateSwapChain();


		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
			const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			const VkAllocationCallbacks* pAllocator,
			VkDebugUtilsMessengerEXT* pDebugMessenger);

		void DestroyDebugUtilsMessengerEXT(VkInstance instance,
			VkDebugUtilsMessengerEXT debugMessenger,
			const VkAllocationCallbacks* pAllocator);

	public:
		TriangleApp() = default;
		TriangleApp(const TriangleApp& app) = default;
		TriangleApp(TriangleApp&& app) = default;
		~TriangleApp() = default;

		void Run();

		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);

		bool	framebufferResized = false;
	};
}