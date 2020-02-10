#pragma once

#include "Context.h"
#include "Shader.h"
#include "InputManager.h"
#include "CommandPool.h"

#include <vector>
#include <optional>

#include "Camera.h"
#include "Mesh.h"
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

#define WIDTH 800
#define HEIGHT 600
#define MAX_FRAMES_IN_FLIGHT 2

#define TEXTURE_PATH "Media/chalet.jpg"

struct UniformBufferObject
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};


namespace Application
{
	class Renderer
	{
	private:
		Context							_context;
		InputManager					_inputManager;
		std::vector<Mesh*>				_meshes;
		float							_lastFrame;
		float							_currentFrameTime;
		GLFWwindow*						_window;
		VkSwapchainKHR					_swapChain;
		std::vector<VkImage>			_swapChainImages;
		VkFormat						_swapChainImageFormat;
		VkExtent2D						_swapChainExtent;
		std::vector<VkImageView>		_swapChainImageViews;
		VkRenderPass					_renderPass;
		VkDescriptorSetLayout			_descriptorSetLayout;
		VkPipelineLayout				_pipelineLayout;
		VkPipeline						_graphicsPipeline;
		std::vector<VkCommandBuffer>	_commandBuffers;
		std::vector<VkSemaphore>		_imageAvailableSemaphores;
		std::vector<VkSemaphore>		_renderFinishedSemaphores;
		std::vector<VkFence>			_inFlightFences;
		std::vector<VkFence>			_imagesInFlight;
		size_t							_currentFrame = 0;
		std::vector<VkFramebuffer>		_swapChainFramebuffers;
		VkDescriptorPool				_descriptorPool;
		VkImage							_depthImage;
		VkDeviceMemory					_depthImageMemory;
		VkImageView						_depthImageView;
		std::vector<Shader>				_shaders;

		struct SwapChainSupportDetails
		{
    		VkSurfaceCapabilitiesKHR capabilities;
    		std::vector<VkSurfaceFormatKHR> formats;
    		std::vector<VkPresentModeKHR> presentModes;
		};

		void InitWindow();
		void InitVulkan();
		void CreateSwapChain();
		void CreateImageViews();
		void CreateRenderPass();
		void CreateDescriptorSetLayout();
		void CreateGraphicsPipeline();
		void CreateFramebuffers();
		VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkFormat FindDepthFormat();
		void CreateDepthResources();
		VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
		void CreateUniformBuffers();
		void CreateDescriptorPool();
		void CreateDescriptorSets();
		void CreateCommandBuffers();
		void CreateSyncObjects();
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
		void RecreateGraphicPipeline();

	public:
		Renderer() = default;
		Renderer(const Renderer& app) = default;
		Renderer(Renderer&& app) = default;
		~Renderer() = default;

		void Run();

		bool	framebufferResized = false;
		bool	shaderChanged = false;
		bool	mouseCaptured = false;

		Camera	cam;

		float	deltaTime;
		bool run = true;

	};
}