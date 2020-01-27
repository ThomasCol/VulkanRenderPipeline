#pragma once

#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

namespace Application
{
	struct Camera
	{
		glm::vec3 position = glm::vec3(-3.0f, 0.5f, 0.0f);
		glm::vec3 front = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		bool firstMouse;
		float yaw;
		float pitch;
		float lastMouseX;
		float lastMouseY;

		void MoveForward(float deltaTime);
		void MoveBackward(float deltaTime);
		void MoveRight(float deltaTime);
		void MoveLeft(float deltaTime);
		void Look(float deltaTime, float mouseX, float mouseY);

		glm::mat4 GetInverseMatrix();
	};
}