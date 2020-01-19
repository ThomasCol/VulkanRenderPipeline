#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <optional>

#define WIDTH 800
#define HEIGHT 600
#define MAX_FRAMES_IN_FLIGHT 2


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
		VkPipelineLayout				_pipelineLayout;
		VkPipeline						_graphicsPipeline;
		VkCommandPool					_commandPool;
		std::vector<VkCommandBuffer>	_commandBuffers;
		std::vector<VkSemaphore>		_imageAvailableSemaphores;
		std::vector<VkSemaphore>		_renderFinishedSemaphores;
		std::vector<VkFence>			_inFlightFences;
		std::vector<VkFence>			_imagesInFlight;
		size_t							_currentFrame = 0;

		std::vector<VkFramebuffer>	_swapChainFramebuffers;

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
		void CreateGraphicsPipeline();
		VkShaderModule CreateShaderModule(const std::vector<char>& code);
		void CreateFramebuffers();
		void CreateCommandPool();
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
		void Cleanup();


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
	};
}