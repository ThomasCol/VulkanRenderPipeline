#include "Camera.h"

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

static const float CAM_MOUSE_SENSITIVITY_X = 0.1f;
static const float CAM_MOUSE_SENSITIVITY_Y = 0.1f;

namespace Application
{
	void Camera::MoveForward(float deltaTime)
	{
		float cameraSpeed = 2.5 * deltaTime;
		position += cameraSpeed * front;
	}

	void Camera::MoveBackward(float deltaTime)
	{
		float cameraSpeed = 2.5 * deltaTime;
		position -= cameraSpeed * front;
	}

	void Camera::MoveLeft(float deltaTime)
	{
		float cameraSpeed = 2.5 * deltaTime;
		position -= glm::normalize(glm::cross(front, up)) * cameraSpeed;
	}

	void Camera::MoveRight(float deltaTime)
	{
		float cameraSpeed = 2.5 * deltaTime;
		position += glm::normalize(glm::cross(front, up)) * cameraSpeed;
	}

	void Camera::Look(float deltaTime, float mouseX, float mouseY)
	{
		if (firstMouse)
		{
			lastMouseX = mouseX;
			lastMouseY = mouseY;
			firstMouse = false;
		}

		float xoffset = mouseX - lastMouseX;
		float yoffset = lastMouseY - mouseY; // reversed since y-coordinates go from bottom to top
		lastMouseX = mouseX;
		lastMouseY = mouseY;

		xoffset *= CAM_MOUSE_SENSITIVITY_X;
		yoffset *= CAM_MOUSE_SENSITIVITY_Y;

		yaw += xoffset;
		pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 frontTmp;
		frontTmp.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		frontTmp.y = sin(glm::radians(pitch));
		frontTmp.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(frontTmp);
	}

	glm::mat4 Camera::GetInverseMatrix()
	{
		// We know how to compute the inverse of the camera
		glm::mat4 ViewTransform = glm::identity<glm::mat4>();
		ViewTransform = glm::lookAt(position, position + front, up);
		return ViewTransform;
	}
}