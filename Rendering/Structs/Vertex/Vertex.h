/**
 * @file Vertex.h
 * @brief Vertex struct header file.
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

#ifndef ALADDIN_3D_STRUCT_VERTEX_H_
#define ALADDIN_3D_STRUCT_VERTEX_H_

#include "glm/glm.hpp"

namespace aladdin_3d {

	/**
	 * @brief A geometry vertex.
	 * 
	 * This Struct represents a vertex of a given geometry and all its attributes.
	 */
	struct Vertex {

		glm::vec3 position;	/// 3D coordinates of the vertex.
		glm::vec3 normal;	/// Normal vector of the vertex.
		glm::vec3 color;	/// Color of the vertex in RGB.
		glm::vec2 uv;		/// UV coordinates to apply textures.

	};

} // namespace aladdin_3d

#endif //!ALADDIN_3D_STRUCT_VERTEX_H_
