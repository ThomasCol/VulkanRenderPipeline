#include "InputManager.h"
#include "Renderer.h"

#include <iostream>

namespace Application
{
	void InputManager::Update(GLFWwindow* window, Renderer* renderer)
	{
		int state = glfwGetKey(window, GLFW_KEY_E);
		if (state == GLFW_PRESS)
		{
			renderer->shaderChanged = true;
		}
		state = glfwGetKey(window, GLFW_KEY_W);
		if (state == GLFW_PRESS)
		{
			renderer->cam.MoveForward(renderer->deltaTime);
		}
		state = glfwGetKey(window, GLFW_KEY_S);
		if (state == GLFW_PRESS)
		{
			renderer->cam.MoveBackward(renderer->deltaTime);
		}
		state = glfwGetKey(window, GLFW_KEY_A);
		if (state == GLFW_PRESS)
		{
			renderer->cam.MoveLeft(renderer->deltaTime);
		}
		state = glfwGetKey(window, GLFW_KEY_D);
		if (state == GLFW_PRESS)
		{
			renderer->cam.MoveRight(renderer->deltaTime);
		}
		state = glfwGetKey(window, GLFW_KEY_ESCAPE);
		if (state == GLFW_PRESS)
		{
			if (renderer->mouseCaptured)
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				renderer->mouseCaptured = false;
			}
			else
			{
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			}
		}

		double mouseX, mouseY;

		glfwGetCursorPos(window, &mouseX, &mouseY);

		renderer->cam.Look(renderer->deltaTime, mouseX, mouseY);
	}
}