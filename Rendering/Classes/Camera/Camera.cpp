/**
 * @file Camera.cpp
 * @brief Camera class implementation file.
 * @version 1.0.0 (2022-11-23)
 * @date 2022-11-23
 * @author Borja Garc�a Quiroga <garcaqub@tcd.ie>
 *
 *
 * Copyright (c) Borja Garc�a Quiroga, All Rights Reserved.
 *
 * The information and material provided below was developed as partial
 * requirements for the MSc in Computer Science at Trinity College Dublin,
 * Ireland.
 */

#include "Camera.h"

#include <iostream>

#include "GL/glew.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtx/string_cast.hpp>

namespace aladdin_3d {

	Camera::Camera(glm::vec3 position, glm::vec3 direction, float fov, float near, float far, int width, int height) {

		// Store all the parameters.
		this->direction = direction;
		this->position = position;
		this->far = far;
		this->fov = fov;
		this->near = near;
		this->window_width = width;
		this->window_height = height;

		// Set defaults.
		this->view = glm::mat4(1.0f);
		this->projection = glm::mat4(1.0f);
		this->camera_matrix = glm::mat4(1.0f);
		this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	
	}

	glm::mat4 Camera::getCameraMatrix() {

		return this->camera_matrix;

	}

	glm::vec3 Camera::getDirection() {

		return this->direction;

	}

	glm::vec3 Camera::getPosition() {

		return this->position;

	}

	glm::mat4 Camera::getProjection() {

		return this->projection;

	}

	glm::vec3 Camera::getUp() {

		return this->up;

	}

	glm::mat4 Camera::getView() {

		return this->view;

	}

	void Camera::moveBack() {

		// Clone the direction.
		glm::vec3 norm_direction = this->direction;

		// Discard the vertical component.
		norm_direction.y = 0;

		// Normalize it.
		norm_direction = glm::normalize(norm_direction);

		// Sum it to the current position.
		this->position -= this->speed * norm_direction;

	}

	void Camera::moveDown() {

		// Get the up direction normalized.
		glm::vec3 norm_up = glm::normalize(this->up);

		// Dif it to the current position.
		this->position -= this->speed * norm_up;

	}

	void Camera::moveFront() {

		// Clone the direction.
		glm::vec3 norm_direction = this->direction;

		// Discard the vertical component.
		norm_direction.y = 0;

		// Normalize it.
		norm_direction = glm::normalize(norm_direction);

		// Sum it to the current position.
		this->position += this->speed * norm_direction;

	}

	void Camera::moveLeft() {

		// Get the normalized up and direction.
		glm::vec3 norm_direction = glm::normalize(this->direction);
		glm::vec3 norm_up = glm::normalize(this->up);

		// Calculate the perpendicular vector using the up vector.
		// The cross product does this.
		glm::vec3 left_direction = -glm::cross(norm_direction, norm_up);
		left_direction = glm::normalize(left_direction);

		// Sum it to the current position.
		this->position += this->speed * left_direction;

	}

	void Camera::moveRight() {

		// Get the normalized up and direction.
		glm::vec3 norm_direction = glm::normalize(this->direction);
		glm::vec3 norm_up = glm::normalize(this->up);

		// Calculate the perpendicular vector using the up vector.
		// The cross product does this.
		glm::vec3 right_direction = glm::cross(norm_direction, norm_up);
		right_direction = glm::normalize(right_direction);

		// Sum it to the current position.
		this->position += this->speed * right_direction;

	}

	void Camera::moveUp() {

		// Get the up direction normalized.
		glm::vec3 norm_up = glm::normalize(this->up);
		
		// Sum it to the current position.
		this->position += this->speed * norm_up;

	}

	void Camera::rotateDown() {

		// Get the looking direction normalized.
		glm::vec3 norm_direction = glm::normalize(this->direction);

		// Build a new direction using the previos values.
		norm_direction.y -= vertical_rotation;

		// Now store it again.
		this->direction = glm::normalize(norm_direction);

	}

	void Camera::rotateLeft() {

		// Get the looking direction normalized.
		glm::vec3 norm_direction = glm::normalize(this->direction);

		// Rotation axis.
		glm::vec3 axis(0.0, 1.0, 0.0);

		// Get the rotation amount.
		float radians = glm::radians(this->horizontal_rotation);

		// Create a rotation matrix.
		glm::mat4 identity_matrix(1.0f);
		glm::mat4 rotation_matrix = glm::rotate(identity_matrix, radians, axis);

		// Apply the rotation.
		norm_direction = glm::vec3(rotation_matrix * glm::vec4(norm_direction, 1.0));

		// Now store it again.
		this->direction = glm::normalize(norm_direction);

	}

	void Camera::rotateRight() {

		// Get the looking direction normalized.
		glm::vec3 norm_direction = glm::normalize(this->direction);

		// Rotation axis.
		glm::vec3 axis(0.0, -1.0, 0.0);

		// Get the rotation amount.
		float radians = glm::radians(this->horizontal_rotation);

		// Create a rotation matrix.
		glm::mat4 identity_matrix(1.0f);
		glm::mat4 rotation_matrix = glm::rotate(identity_matrix, radians, axis);

		// Apply the rotation.
		norm_direction = glm::vec3(rotation_matrix * glm::vec4(norm_direction, 1.0));

		// Now store it again.
		this->direction = glm::normalize(norm_direction);

	}

	void Camera::rotateUp() {

		// Get the looking direction normalized.
		glm::vec3 norm_direction = glm::normalize(this->direction);

		// Build a new direction using the previos values.
		norm_direction.y += vertical_rotation;

		// Now store it again.
		this->direction = glm::normalize(norm_direction);

	}

	void Camera::update() {

		// Calculate the view matrix.
		this->view = glm::lookAt(this->position, this->position + this->direction, this->up);

		// Adds perspective to the scene.
		float ratio = (float)this->window_width / this->window_height;
		this->projection = glm::perspective(glm::radians(this->fov), ratio, this->near, this->far);

		// Stores new camera matrix.
		this->camera_matrix = projection * view;

	}

}  // namespace aladdin_3d
