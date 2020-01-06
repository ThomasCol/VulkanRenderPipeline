#include <functional>
#include <cstdlib>
#include <stdexcept>
#include <iostream>

#include "TriangleApp.h"


namespace Application
{
	void TriangleApp::InitWindow()
	{
		if (glfwInit() == GLFW_FALSE)
			throw std::runtime_error("failed to init window");
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	}

	void TriangleApp::InitVulkan()
	{
		CreateInstance();
	}

	void TriangleApp::CreateInstance()
	{
		if (enableValidationLayers && !CheckValidationLayerSupport())
			throw std::runtime_error("validation layers requested, but not available");

		VkApplicationInfo appInfo {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Triangle App";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;
		createInfo.enabledLayerCount = 0;

		if (vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS)
			throw std::runtime_error("failed to create instance!");

		CheckExtension();
	}

	void TriangleApp::CheckExtension()
	{
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> extensions(extensionCount);

		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		const char** requiredExtension{ glfwGetRequiredInstanceExtensions(&extensionCount) };

		bool extensionFound { false };

		for (uint32_t i = 0; i < extensionCount; i++)
		{
			extensionFound = false;
			for (size_t j = 0; j < extensions.size(); j++)
			{
				if (strcmp(extensions[j].extensionName, requiredExtension[i]) == 0)
				{
					extensionFound = true;
					break;
				}
			}

			if (!extensionFound)
				throw std::runtime_error("missing required extensions");
		}
	}

	bool TriangleApp::CheckValidationLayerSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const auto& layerName : _validationLayers)
		{
			bool layerFound = false;
			for (const auto& layerProperties : availableLayers)
			{
				std::cout << layerProperties.layerName << std::endl;
				if (strcmp(layerName, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
				return false;
		}

		return true;
	}

	void TriangleApp::MainLoop()
	{
		while (!glfwWindowShouldClose(_window))
		{
			glfwPollEvents();
		}
	}

	void TriangleApp::Cleanup()
	{
		vkDestroyInstance(_instance, nullptr);

		glfwDestroyWindow(_window);

		glfwTerminate();
	}

	void TriangleApp::Run()
	{
		InitWindow();
		InitVulkan();
		MainLoop();
		Cleanup();
	}
}