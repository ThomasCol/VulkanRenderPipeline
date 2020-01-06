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
		GLFWwindow* _window;
		VkInstance	_instance;

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
		void CheckExtension();
		bool CheckValidationLayerSupport();
		void MainLoop();
		void Cleanup();

	public:
		TriangleApp() = default;
		TriangleApp(const TriangleApp& app) = default;
		TriangleApp(TriangleApp&& app) = default;
		~TriangleApp() = default;

		void Run();
	};
}