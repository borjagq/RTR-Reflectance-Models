/**
 * @file BoundingBox.h
 * @brief BoundingBox struct header file.
 * @version 1.0.0 (2023-01-02)
 * @date 2023-01-02
 * @author Borja García Quiroga <garcaqub@tcd.ie>
 *
 *
 * Copyright (c) Borja García Quiroga, All Rights Reserved.
 *
 * The information and material provided below was developed as partial
 * requirements for the MSc in Computer Science at Trinity College Dublin,
 * Ireland.
 */

#ifndef ALADDIN_3D_STRUCT_BOUNDINGBOX_H_
#define ALADDIN_3D_STRUCT_BOUNDINGBOX_H_

#include "glm/glm.hpp"

namespace aladdin_3d {

	/**
	 * @brief A bounding box struct.
	 *
	 * This Struct represents the bounding box of an object.
	 */
	struct BoundingBox {

		glm::vec3 min;	// Minimum vertex.
		glm::vec3 max; // Maximum vertex.

	};

} // namespace aladdin_3d

#endif //!ALADDIN_3D_STRUCT_BOUNDINGBOX_H_
