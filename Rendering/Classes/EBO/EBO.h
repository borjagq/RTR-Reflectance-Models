/**
 * @file EBO.h
 * @brief EBO class header file.
 * @version 1.0.0 (2022-10-21)
 * @date 2022-10-21
 * @author Borja García Quiroga <garcaqub@tcd.ie>
 *
 *
 * Copyright (c) Borja García Quiroga, All Rights Reserved.
 *
 * The information and material provided below was developed as partial
 * requirements for the MSc in Computer Science at Trinity College Dublin,
 * Ireland.
 */

#ifndef ALADDIN_3D_CLASS_EBO_H_
#define ALADDIN_3D_CLASS_EBO_H_

#include <vector>

#include "GL/glew.h"

namespace aladdin_3d {

	/**
	 * @brief Implementation of a EBO class.
	 *
	 * Implementation of a EBO class that will allow us to bind it to the
	 * OpenGL pipe, destroy it or deactivate it.
	 *
	 * @author Borja García Quiroga <garcaqub@tcd.ie>
	 */
	class EBO {
		
		public:
			
			/**
			 * @brief Constructs a Elements Buffer Object.
			 *
			 * Constructs a Elements Buffer Object and links its vertices.
			 *
			 * @param indices Indices that will be linked.
			 */
			EBO(const std::vector<GLuint> &indices);

			/**
			 * @brief Binds the EBO.
			 *
			 * Binds the EBO in the GL pipe.
			 */
			void bind();

			/**
			 * @brief Removes the EBO.
			 *
			 * Removes the EBO from OpenGL.
			 */
			void remove();

			/**
			 * @brief Unbinds the EBO.
			 *
			 * Unbinds the EBO in the GL pipe.
			 */
			void unbind();

		private:

			GLuint ID; // GL ID of the EBO.

	};

}  // namespace aladdin_3d

#endif //!ALADDIN_3D_CLASS_EBO_H_
