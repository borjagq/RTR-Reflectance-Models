/**
 * @file LoaderGLTH.h
 * @brief Loader GLTF class header file.
 * @version 1.0.0 (2022-11-27)
 * @date 2022-11-27
 * @author Borja García Quiroga <garcaqub@tcd.ie>
 *
 *
 * Copyright (c) Borja García Quiroga, All Rights Reserved.
 *
 * The information and material provided below was developed as partial
 * requirements for the MSc in Computer Science at Trinity College Dublin,
 * Ireland.
 */

#ifndef ALADDIN_3D_CLASSES_LOADER_GLTF_H_
#define ALADDIN_3D_CLASSES_LOADER_GLTF_H_

#include "Classes/Loader/Loader.h"

#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "json/json.h"

namespace aladdin_3d {

	/**
	 * @brief Implements a GLTF Loader class.
	 *
	 * Implements a loader class that will allow us to load GLTF models.
	 *
	 * @author Borja García Quiroga <garcaqub@tcd.ie>
	 */
	class LoaderGLTF : public Loader {

		public:

			/**
			 * @brief Build a loader instance.
			 *
			 * Build a loader instance.
			 */
			LoaderGLTF(const char* filename);

			/**
			 * @brief Get the geometries from the loaded model.
			 * 
			 * Get the geometries from the loaded model.
			 * 
			 * @param geoms Outputs the geometries returned.
			 * @param matrices Outputs the transformation matrices.
			 */
			void getGeometries(std::vector<Geometry> *geoms, std::vector<glm::mat4> *matrices);

			/**
			 * @brief Loads the data from the file.
		 	 *
			 * Loads the data from the file.
			 */
			void loadModel();

		private:

			/**
			 * @brief Loads a mesh by its index.
			 *
			 * Loads a mesh by its index.
			 *
			 * @param indMesh the index of the mesh.
			 */
			void loadGeometry(unsigned int indMesh);

			/**
			 * @brief Traverses the nodes in the file.
			 *
			 * Traverses the nodes in the file.
			 *
			 * @param nextNode The index of the next node.
			 * @param matrix The matrx that will be used.
			 */
			void recursiveGetNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

			// Interprets the binary data into floats, indices, and textures
			std::vector<float> getFloats(nlohmann::json accessor);
			std::vector<GLuint> getIndices(nlohmann::json accessor);
			std::vector<Texture> getTextures();

			// Assembles all the floats into vertices
			std::vector<Vertex> assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texUVs);

			// Helps with the assembly from above by grouping floats
			std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
			std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
			std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);

			std::vector<unsigned char> bin_data;	/// Binary data stored for convenience.
			nlohmann::json json_file;				/// The model JSON file contents.

	};

}

#endif  //!ALADDIN_3D_CLASSES_LOADER_GLTF_H_

