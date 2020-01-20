#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <optional>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <array>

#define WIDTH 800
#define HEIGHT 600
#define MAX_FRAMES_IN_FLIGHT 2

struct Vertex {
	glm::vec2 pos;
	glm::vec3 color;

	static VkVertexInputBindingDescription GetBindingDescription();

	static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions();
};

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
		VkBuffer						_vertexBuffer;
		VkDeviceMemory					_vertexBufferMemory;
		VkBuffer						_indexBuffer;
		VkDeviceMemory					_indexBufferMemory;
		std::vector<VkBuffer>			_uniformBuffers;
		std::vector<VkDeviceMemory>		_uniformBuffersMemory;
		VkDescriptorPool				_descriptorPool;
		std::vector<VkDescriptorSet>	_descriptorSets;

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

		const std::vector<Vertex> _vertices = {
			{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
			{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
		};

		const std::vector<uint16_t> indices = {
			0, 1, 2, 2, 3, 0
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
		void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void CreateVertexBuffer();
		void CreateIndexBuffer();
		void CreateUniformBuffers();
		void CreateDescriptorPool();
		void CreateDescriptorSets();
		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		void CreateCommandBuffers();
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