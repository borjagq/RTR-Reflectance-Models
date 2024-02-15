/**
 * @file LoaderAssimp.cpp
 * @brief Loader class implementation file using Assimp library.
 * @version 1.0.0 (2023-01-30)
 * @date 2023-01-30
 * @author Borja García Quiroga <garcaqub@tcd.ie>
 *
 *
 * Copyright (c) Borja García Quiroga, All Rights Reserved.
 *
 * The information and material provided below was developed as partial
 * requirements for the MSc in Computer Science at Trinity College Dublin,
 * Ireland.
 */

#include "LoaderAssimp.h"
#include "Classes/Loader/Loader.h"

#include <vector>
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Structs/Vertex/Vertex.h"

namespace aladdin_3d {

	LoaderAssimp::LoaderAssimp(const char* filename) : Loader(filename) {}

	void LoaderAssimp::loadModel() {
        
        // Import the scene from the file.
        const aiScene* scene = aiImportFile(filename, aiProcess_Triangulate | aiProcess_PreTransformVertices);

        // Check if the scene was not read correctly.
        if (!scene) {
            
            std::cerr << "Could not read mesh on file " << filename << std::endl;
            exit(1);
            
        }

        // Print info from the scene.
        std::cerr << "  " << filename << std::endl;
        std::cerr << "  " << scene->mNumMaterials << " materials" << std::endl;
        std::cerr << "  " << scene->mNumMeshes << " meshes" << std::endl;
        std::cerr << "  " << scene->mNumTextures << " textures" << std::endl;
        
        // Loop through the different meshes in the scene.
        for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
            
            // Get the mesh itself.
            const aiMesh* mesh = scene->mMeshes[i];
            
            // Load this mesh.
            loadGeometry(scene, mesh);
            
        }

        aiReleaseImport(scene);

	}

	void LoaderAssimp::loadGeometry(const aiScene* scene, const aiMesh* mesh) {
        
        // Init the ds.
        std::vector<aladdin_3d::Vertex> vertices(0);
        std::vector<GLuint> indices(0);
        
        // If has materials, pass them.
        aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
        aiColor3D color;
        mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        float shine = 0.0;
        mat->Get(AI_MATKEY_SHININESS, shine);
        
        // Iterate through the vertices in the mesh.
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            
            // Build an empty vertex.
            aladdin_3d::Vertex vertex{
                glm::vec3(0.0f, 0.0f, 0.0f),    // Position.
                glm::vec3(1.0f, 1.0f, 1.0f),    // Normal.
                glm::vec3(1.0f, 1.0f, 1.0f),    // Color.
                glm::vec2(0.0f, 0.0f)           // UV coords.
            };
            
            // If it has a position, subtitute it in the vertex.
            if (mesh->HasPositions()) {
                
                const aiVector3D* vp = &(mesh->mVertices[i]);
                vertex.position = glm::vec3(vp->x, vp->y, vp->z);
                
            }
            
            vertex.color = glm::vec3(color.r, color.g, color.b);
            
            // If it has a normal, subtitute it in the vertex.
            if (mesh->HasNormals()) {
                
                const aiVector3D* vn = &(mesh->mNormals[i]);
                vertex.normal = glm::vec3(vn->x, vn->y, vn->z);
                
            }
            
            // If it has UV textures, subtitute it in the vertex.
            if (mesh->HasTextureCoords(0)) {
                
                const aiVector3D* vt = &(mesh->mTextureCoords[0][i]);
                vertex.uv = glm::vec2(vt->x, vt->y);
                
            }
            
            // Add that to vertices.
            vertices.push_back(vertex);

        }
        
        // Lets store all the indices or faces.
        for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
            
            indices.push_back(mesh->mFaces[j].mIndices[0]);
            indices.push_back(mesh->mFaces[j].mIndices[1]);
            indices.push_back(mesh->mFaces[j].mIndices[2]);
        
        }

		// Obtain the textures.
		std::vector<aladdin_3d::Texture> textures = getTextures();

		// Create a Geometry object that contains all this data.
		this->geometries.push_back(aladdin_3d::Geometry(vertices, indices, textures, shine));

	}

	std::vector<aladdin_3d::Texture> LoaderAssimp::getTextures() {

		// Init the textures holder.
		std::vector<aladdin_3d::Texture> textures;

		return textures;

	}

	void LoaderAssimp::getGeometries(std::vector<Geometry> *geoms, std::vector<glm::mat4> *matrices) {

		(*geoms) = this->geometries;
		(*matrices) = this->transform_matrixes;

	}

}
