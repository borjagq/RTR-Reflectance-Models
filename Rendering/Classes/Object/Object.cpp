/**
 * @file Object.cpp
 * @brief Object class implementation file.
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

#include "Object.h"

#include <cassert>
#include <iostream>

#include "json/json.h"

#include "Classes/Loader/Loader.h"
#include "Classes/LoaderGLTF/LoaderGLTF.h"
#include "Classes/LoaderAssimp/LoaderAssimp.h"
#include "Structs/Vertex/Vertex.h"
#include "Structs/BoundingBox/BoundingBox.h"

namespace aladdin_3d {

	Object::Object(const char *filename, const char* filetype) {

		// Build the loader for this file type.
		Loader *model_loader;

		// Choose the specific loader type.
        if (strcmp(filetype, "GLTF") == 0) {
            
            model_loader = new LoaderGLTF(filename);

        } else if (strcmp(filetype, "Assimp") == 0) {
            
            std::cerr << "assimp" << std::endl;

            model_loader = new LoaderAssimp(filename);

        } else {

			assert(false);
			exit(1);

		}
		
        (*model_loader).loadModel();

		(*model_loader).getGeometries(&this->geoms, &this->matrices_geoms);

	}

	Object::Object(std::vector<Geometry> geometries) {

		// Copy the geometries.
		this->geoms = geometries;

		// Get an identity matrix for each.
		this->matrices_geoms = std::vector<glm::mat4>(geometries.size(), glm::mat4(1.0f));

	}

	void Object::draw(aladdin_3d::Shader& shader, aladdin_3d::Camera& camera)
	{
		// Go over all meshes and draw each one
		for (unsigned int i = 0; i < this->geoms.size(); i++)
		{
			geoms[i].draw(shader, camera);
		}
	}

	BoundingBox Object::getBoundingBox() {

		// Create the bb.
		BoundingBox global_bb = this->geoms[0].getBoundingBox();

		// Loop through the vertices and get tge min and max values.
		for (int i = 1; i < this->geoms.size(); i++) {

			// Get the current bb.
			BoundingBox bb = this->geoms[i].getBoundingBox();

			if (global_bb.min.x > bb.min.x)
				global_bb.min.x = bb.min.x;

			if (global_bb.min.y > bb.min.y)
				global_bb.min.y = bb.min.y;

			if (global_bb.min.z > bb.min.z)
				global_bb.min.z = bb.min.z;

			if (global_bb.max.x < bb.max.x)
				global_bb.max.x = bb.max.x;

			if (global_bb.max.y < bb.max.y)
				global_bb.max.y = bb.max.y;

			if (global_bb.max.z < bb.max.z)
				global_bb.max.z = bb.max.z;

		}

		return global_bb;

	}

	std::vector<Geometry> Object::getGeometries() {

		return this->geoms;

	}

	std::vector<glm::mat4> Object::getGeometryMatrices() {

		return this->matrices_geoms;

	}

    void Object::setShininess(float shine) {
        
        for (int i = 0; i < this->geoms.size(); i++) {
            this->geoms[i].setShininess(shine);
        }
        
    }

	void Object::resetTransforms() {

		// Do the same for the subobjects.
		for (size_t i = 0; i < geoms.size(); i++) {

			geoms[i].resetTransforms();

		}

	}

	void Object::rotate(float x, float y, float z, float angle) {

		// Do the same for the subobjects.
		for (size_t i = 0; i < geoms.size(); i++) {

			geoms[i].rotate(x, y, z, angle);

		}

	}

	void Object::rotate(int num, float x, float y, float z, float angle) {

		geoms[num].rotate(x, y, z, angle);

	}

	void Object::scale(float x, float y, float z) {

		// Do the same for the subobjects.
		for (size_t i = 0; i < geoms.size(); i++) {

			geoms[i].scale(x, y, z);

		}

	}

	void Object::scale(int num, float x, float y, float z) {

		geoms[num].scale(x, y, z);

	}

	void Object::translate(float x, float y, float z) {

		// Do the same for the subobjects.
		for (size_t i = 0; i < geoms.size(); i++) {

			geoms[i].translate(x, y, z);

		}

	}

	void Object::translate(int num, float x, float y, float z) {

		geoms[num].translate(x, y, z);

	}

}
