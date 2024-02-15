/**
 * @file Light.h
 * @brief Light class header file.
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

#ifndef ALADDIN_3D_CLASSES_LIGHT_H_
#define ALADDIN_3D_CLASSES_LIGHT_H_

#include "GL/glew.h"
#include "glm/glm.hpp"

namespace aladdin_3d {

	/**
	 * @brief Implementation of a Light class.
	 *
	 * Implementation of a Light class that will allow us light the scenes up.
	 *
	 * @author Borja Garc�a Quiroga <garcaqub@tcd.ie>
	 */
	class Light {

	public:
        
        /**
         * @brief Constructs a Light.
         *
         * Constructs a Light that will not illuminate.
         *
         */
        Light();

		/**
		 * @brief Constructs a Light from its components.
		 *
		 * Constructs a Light from its components.
		 *
		 * @param light_pos The position of the light.
		 * @param light_color The light color.
		 */
		Light(glm::vec3 light_pos, glm::vec4 light_color);

		/**
		 * @brief Get the color of the light.
		 * 
		 * Get the color of the light.
		 */
		glm::vec4 getColor();

		/**
		 * @brief Get the position of the light.
		 * 
		 * Get the position of the light.
		 */
		glm::vec3 getPosition();

	private:

		glm::vec3 position;
		glm::vec4 color;

	};

}  // namespace aladdin_3d

#endif //!ALADDIN_3D_CLASSES_LIGHT_H_
