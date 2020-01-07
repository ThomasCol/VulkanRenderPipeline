#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#define WIDTH 800
#define HEIGHT 600


namespace Application
{
	class TriangleApp
	{
	private:
		GLFWwindow*					_window;
		VkInstance					_instance;
		VkDebugUtilsMessengerEXT	_debugMessenger;
		VkPhysicalDevice			_physicalDevice { VK_NULL_HANDLE };

		const std::vector<const char*> _validationLayers {
			"VK_LAYER_KHRONOS_validation"
		};

#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif

		void InitWindow();
		void InitVulkan();
		void CreateInstance();
		void SetupDebugMessenger();
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void CheckExtension();
		std::vector<const char*> GetRequiredEtensions();
		bool CheckValidationLayerSupport();
		void PickPhysicalDevice();
		bool IsDeviceSuitable();
		void MainLoop();
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