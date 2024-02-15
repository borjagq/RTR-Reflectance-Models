/**
 * @file Light.cpp
 * @brief Light class implementation file.
 * @version 1.0.0 (2022-10-27)
 * @date 2022-10-27
 * @author Borja Garc�a Quiroga <garcaqub@tcd.ie>
 *
 *
 * Copyright (c) Borja Garc�a Quiroga, All Rights Reserved.
 *
 * The information and material provided below was developed as partial
 * requirements for the MSc in Computer Science at Trinity College Dublin,
 * Ireland.
 */

#include "Light.h"

#include "GL/glew.h"

namespace aladdin_3d {

    Light::Light() {

        // Copy these into the attributes.
        this->position = glm::vec3(0.0f, 0.0f, 0.0f);
        this->color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

    }

	Light::Light(glm::vec3 light_pos, glm::vec4 light_color) {

		// Copy these into the attributes.
		this->position = light_pos;
		this->color = light_color;

	}


	glm::vec4 Light::getColor() {

		return this->color;

	}

	glm::vec3 Light::getPosition() {

		return this->position;

	}

}  // namespace aladdin_3d
