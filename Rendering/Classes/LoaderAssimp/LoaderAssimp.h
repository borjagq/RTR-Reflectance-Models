/**
 * @file LoaderAssimp.h
 * @brief Loader GLTF class header file.
 * @version 1.0.0 (2022-11-27)
 * @date 2022-11-27
 * @author Borja Garc�a Quiroga <garcaqub@tcd.ie>
 *
 *
 * Copyright (c) Borja Garc�a Quiroga, All Rights Reserved.
 *
 * The information and material provided below was developed as partial
 * requirements for the MSc in Computer Science at Trinity College Dublin,
 * Ireland.
 */

#ifndef ALADDIN_3D_CLASSES_LOADER_ASSIMP_H_
#define ALADDIN_3D_CLASSES_LOADER_ASSIMP_H_

#include "Classes/Loader/Loader.h"

#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace aladdin_3d {

	/**
	 * @brief Implements a GLTF Loader class.
	 *
	 * Implements a loader class that will allow us to load GLTF models.
	 *
	 * @author Borja Garc�a Quiroga <garcaqub@tcd.ie>
	 */
	class LoaderAssimp : public Loader {

		public:

			/**
			 * @brief Build a loader instance.
			 *
			 * Build a loader instance.
			 */
			LoaderAssimp(const char* filename);

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
			 * @param mesh the assimp  mesh.
			 */
			void loadGeometry(const aiScene* scene, const aiMesh* mesh);
        
            /**
             * @brief Load the textures for this model.
             *
             * Load the textures for this model.
             */
            std::vector<aladdin_3d::Texture> getTextures();

	};

}

#endif  //!ALADDIN_3D_CLASSES_LOADER_ASSIMP_H_

