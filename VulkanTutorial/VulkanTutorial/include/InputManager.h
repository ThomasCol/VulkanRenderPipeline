#pragma once


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Application
{
	class Renderer;

	class InputManager
	{
	public:
		void Update(GLFWwindow* window, Renderer* renderer);
	};
}