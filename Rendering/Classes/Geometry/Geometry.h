/**
 * @file Geometry.h
 * @brief Geometry class header file.
 * @version 1.0.0 (2022-11-26)
 * @date 2022-11-26
 * @author Borja García Quiroga <garcaqub@tcd.ie>
 *
 *
 * Copyright (c) Borja García Quiroga, All Rights Reserved.
 *
 * The information and material provided below was developed as partial
 * requirements for the MSc in Computer Science at Trinity College Dublin,
 * Ireland.
 */

#ifndef ALADDIN_3D_CLASSES_GEOMETRY_H_
#define ALADDIN_3D_CLASSES_GEOMETRY_H_

#include <vector>

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Classes/Camera/Camera.h"
#include "Classes/Shader/Shader.h"
#include "Classes/Texture/Texture.h"
#include "Classes/EBO/EBO.h"
#include "Classes/VBO/VBO.h"
#include "Classes/VAO/VAO.h"
#include "Structs/Vertex/Vertex.h"
#include "Structs/BoundingBox/BoundingBox.h"

namespace aladdin_3d {

	/**
	* @brief Implementation of a Geometry class.
	*
	* Implementation of a Geometry class that will allow us to handle the geometric part
	* of the objects in the VBOs.
	*
	* @author Borja García Quiroga <garcaqub@tcd.ie>
	*/
	class Geometry {

		public:
			
			/**
			 * @brief Initializes the Geometry.
			 * 
			 * Initializes the geometry and stores it.
			 * 
			 * @param vertices Vertices of the object.
			 * @param indices Indices of the vertices.
			 * @param textures Textures in connection with this geometry.
			 */
			Geometry(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices, const std::vector<Texture> &textures, const float shininess);

			/**
			 * @brief Get the indices of the geometry.
			 *
			 * Get the indices of the geometry.
			 */
			std::vector<GLuint> getIndices();
			
			/**
			 * @brief Get the textures.
			 *
			 * Get the textures.
			 */
			std::vector<Texture> getTextures();
			
			/**
			 * @brief Get the VAO.
			 *
			 * Get the VAO.
			 */
			VAO getVAO();
			
			/**
			 * @brief Get the vertices of the geometry.
			 *
			 * Get the vertices of the geometry.
			 */
			std::vector<Vertex> getVertices();
        
            /**
             * @brief Get the object shininess.
             *
             * Get the object shininess.
             */
            float getShininess();
        
            /**
             * @brief Set the object shininess.
             *
             * Set the object shininess.
             */
            void setShininess(float shine);

			/**
			 * @brief Draws the Geometry.
			 *
			 * Displays the Geometry in OpenGL.
			 */
			void draw(Shader &shader, Camera &camera);

			/**
			 * @brief Gets the bounding box.
			 * 
			 * Gets the bounding box of the geometry.
			 * 
			 * @returns The bounding box struct.
			 */
			BoundingBox getBoundingBox();

			/**
			 * @brief Reset
			 *
			 * Add a translation matrix to the model.
			 */
			void resetTransforms();

			/**
			 * @brief Add a translation matrix to the model.
			 *
			 * Add a translation matrix to the model.
			 *
			 * @param x The x rotation.
			 * @param y The y rotation.
			 * @param z The z rotation.
			 * @param angle The angle to rotate.
			 */
			void rotate(float x, float y, float z, float angle);

			/**
			 * @brief Add a translation matrix to the model.
			 *
			 * Add a translation matrix to the model.
			 *
			 * @param x The x scale.
			 * @param y The y scale.
			 * @param z The z scale.
			 */
			void scale(float x, float y, float z);

			/**
			 * @brief Add a translation matrix to the model.
			 *
			 * Add a translation matrix to the model.
			 *
			 * @param x The x translation.
			 * @param y The y translation.
			 * @param z The z translation.
			 */
			void translate(float x, float y, float z);

		private:

			/**
			 * @brief Updates the normal matrix.
			 * 
			 * Updates the normal matrix so it can be passed to the shader.
			 */
			void updateNormalMatrix();

			std::vector<GLuint> indices;				/// Indices of the vertices.
			std::vector<Texture> textures;				/// Textures that will color this geometry.
			VAO vao;									/// VAO containing this object.
			std::vector<Vertex> vertices;				/// Geometry vertices.
			glm::mat4 transforms = glm::mat4(1.0f);		/// Tranform matrixes that will be passed to the shader.
            float shininess = 1.0;

	};

}  // namespace aladdin_3d

#endif  //! ALADDIN_3D_CLASSES_GEOMETRY_H_
