/**
 * @file Loader.cpp
 * @brief Loader class implementation file.
 * @version 1.0.0 (2022-11-27)
 * @date 2022-11-27
 * @author Borja García Quiroga <garcaqub@tcd.ie>
 * 
 * The code in this class has been partially based on the OpenGL Tutorials code.
 * The auxiliary functions have been grabbed from the repository below and belong to
 * Victor Gordan.
 * @see https://github.com/VictorGordan/opengl-tutorials
 *
 *
 * Copyright (c) Borja García Quiroga, All Rights Reserved.
 *
 * The information and material provided below was developed as partial
 * requirements for the MSc in Computer Science at Trinity College Dublin,
 * Ireland.
 */

#include "LoaderGLTF.h"
#include "Classes/Loader/Loader.h"

#include <vector>
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "json/json.h"

namespace aladdin_3d {

	LoaderGLTF::LoaderGLTF(const char* filename) : Loader(filename) {}

	void LoaderGLTF::loadModel() {
        
		// Parse the JSON contents of the file.
		std::string text = Loader::readFileContents(filename);
		this->json_file = nlohmann::json::parse(text);

		// Get the URI of the data file. That file contains the real info.
		std::string bin_file = json_file["buffers"][0]["uri"];

		// Substring the file name to get the directory path.
		std::string filename_str(filename);
		std::string file_dir_path = filename_str.substr(0, filename_str.find_last_of('/') + 1);

		// Read the contents of the bin file in the directory.
		std::string raw_bytes = Loader::readFileContents((file_dir_path + bin_file).c_str());

		// Transform the raw text data into bytes and put them in a vector
		this->bin_data = std::vector<unsigned char>(raw_bytes.begin(), raw_bytes.end());

		// Now, iterate the nodes in the file recursively.
		recursiveGetNode(0);

	}

	void LoaderGLTF::recursiveGetNode(unsigned int nextNode, glm::mat4 matrix) {

		// Get the current node.
		nlohmann::json node = json_file["nodes"][nextNode];

		// GLTF files can have transformations built in.

		// Get the translation for each element.
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
		if (node.find("translation") != node.end())	
			translation = glm::vec3(node["translation"][0], node["translation"][1], node["translation"][2]);

		// Get quaternion if it exists
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		if (node.find("rotation") != node.end())
		{
			float rotValues[4] =
			{
				node["rotation"][3],
				node["rotation"][0],
				node["rotation"][1],
				node["rotation"][2]
			};

			rotation = glm::make_quat(rotValues);
		}

		// Get scale if it exists
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

		if (node.find("scale") != node.end())
			scale = glm::vec3(node["scale"][0], node["scale"][1], node["scale"][2]);

		// Get matrix if it exists
		glm::mat4 matNode = glm::mat4(1.0f);
		if (node.find("matrix") != node.end())
		{
			float matValues[16];
			for (unsigned int i = 0; i < node["matrix"].size(); i++)
				matValues[i] = (node["matrix"][i]);
			matNode = glm::make_mat4(matValues);
		}

		// Initialize matrices.
		glm::mat4 trans = glm::mat4(1.0f);
		glm::mat4 rot = glm::mat4(1.0f);
		glm::mat4 sca = glm::mat4(1.0f);

		// Use translation, rotation, and scale to change the initialized matrices
		trans = glm::translate(trans, translation);
		rot = glm::mat4_cast(rotation);
		sca = glm::scale(sca, scale);

		// Multiply all matrices together to obtain the final transform.
		glm::mat4 transform_matrix = matrix * matNode * trans * rot * sca;

		// Check if the node contains a mesh and if it does load it
		if (node.find("mesh") != node.end()) {

			this->transform_matrixes.push_back(transform_matrix);

			loadGeometry(node["mesh"]);

		}

		// Check if the node has children, and if it does, apply this function to them with the matNextNode
		if (node.find("children") != node.end()) {

			for (unsigned int i = 0; i < node["children"].size(); i++)
				recursiveGetNode(node["children"][i], transform_matrix);

		}

	}

	void LoaderGLTF::loadGeometry(unsigned int indMesh) {

		// These indices will tell us what data to grab.
		unsigned int posAccInd = this->json_file["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
		unsigned int normalAccInd = this->json_file["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
		unsigned int texAccInd = this->json_file["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
		unsigned int indAccInd = this->json_file["meshes"][indMesh]["primitives"][0]["indices"];

		// Get each type of information using the get functions.
		std::vector<float> posVec = getFloats(this->json_file["accessors"][posAccInd]);
		std::vector<glm::vec3> positions = groupFloatsVec3(posVec);
		std::vector<float> normalVec = getFloats(this->json_file["accessors"][normalAccInd]);
		std::vector<glm::vec3> normals = groupFloatsVec3(normalVec);
		std::vector<float> texVec = getFloats(this->json_file["accessors"][texAccInd]);
		std::vector<glm::vec2> texUVs = groupFloatsVec2(texVec);
        
		// Combine all that data into the vertices.
		std::vector<aladdin_3d::Vertex> vertices = assembleVertices(positions, normals, texUVs);

		// Grab the indices.
		std::vector<GLuint> indices = getIndices(this->json_file["accessors"][indAccInd]);

		// Obtain the textures.
		std::vector<aladdin_3d::Texture> textures = getTextures();

		// Create a Geometry object that contains all this data.
		this->geometries.push_back(aladdin_3d::Geometry(vertices, indices, textures, 1.0));

	}

	std::vector<float> LoaderGLTF::getFloats(nlohmann::json accessor) {

		std::vector<float> floatVec;

		// Get properties from the accessor
		unsigned int buffViewInd = accessor.value("bufferView", 1);
		unsigned int count = accessor["count"];
		unsigned int accByteOffset = accessor.value("byteOffset", 0);
		std::string type = accessor["type"];

		// Get properties from the bufferView
		nlohmann::json bufferView = this->json_file["bufferViews"][buffViewInd];
		unsigned int byteOffset = bufferView["byteOffset"];

		// Interpret the type and store it into numPerVert
		unsigned int numPerVert;
		if (type == "SCALAR") numPerVert = 1;
		else if (type == "VEC2") numPerVert = 2;
		else if (type == "VEC3") numPerVert = 3;
		else if (type == "VEC4") numPerVert = 4;
		else throw std::invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3, or VEC4)");

		// Go over all the bytes in the data at the correct place using the properties from above
		unsigned int beginningOfData = byteOffset + accByteOffset;
		unsigned int lengthOfData = count * 4 * numPerVert;
		for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i)
		{
			unsigned char bytes[] = { this->bin_data[i++], this->bin_data[i++], this->bin_data[i++], this->bin_data[i++] };
			float value;
			std::memcpy(&value, bytes, sizeof(float));
			floatVec.push_back(value);
		}

		return floatVec;
	}

	std::vector<GLuint> LoaderGLTF::getIndices(nlohmann::json accessor) {

		std::vector<GLuint> indices;

		// Get properties from the accessor
		unsigned int buffViewInd = accessor.value("bufferView", 0);
		unsigned int count = accessor["count"];
		unsigned int accByteOffset = accessor.value("byteOffset", 0);
		unsigned int componentType = accessor["componentType"];

		// Get properties from the bufferView
		nlohmann::json bufferView = this->json_file["bufferViews"][buffViewInd];
		unsigned int byteOffset = bufferView["byteOffset"];

		// Get indices with regards to their type: unsigned int, unsigned short, or short
		unsigned int beginningOfData = byteOffset + accByteOffset;
		if (componentType == 5125)
		{
			for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 4; i)
			{
				unsigned char bytes[] = { this->bin_data[i++], this->bin_data[i++], this->bin_data[i++], this->bin_data[i++] };
				unsigned int value;
				std::memcpy(&value, bytes, sizeof(unsigned int));
				indices.push_back((GLuint)value);
			}
		}
		else if (componentType == 5123)
		{
			for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
			{
				unsigned char bytes[] = { this->bin_data[i++], this->bin_data[i++] };
				unsigned short value;
				std::memcpy(&value, bytes, sizeof(unsigned short));
				indices.push_back((GLuint)value);
			}
		}
		else if (componentType == 5122)
		{
			for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
			{
				unsigned char bytes[] = { this->bin_data[i++], this->bin_data[i++] };
				short value;
				std::memcpy(&value, bytes, sizeof(short));
				indices.push_back((GLuint)value);
			}
		}

		return indices;
	}

	std::vector<aladdin_3d::Texture> LoaderGLTF::getTextures() {

		// Init the textures holder.
		std::vector<aladdin_3d::Texture> textures;

		// Get the path to this model.
		std::string filename_str(filename);
		std::string file_dir_path = filename_str.substr(0, filename_str.find_last_of('/') + 1);

		// Go through all materials.
		for (size_t i = 0; i < this->json_file["materials"].size(); i++) {

			// Get the textures data.
			std::string material_name = this->json_file["materials"][i]["name"];
			int texture_index = this->json_file["materials"][i]["pbrMetallicRoughness"]["baseColorTexture"]["index"];
			std::string texture_uri = this->json_file["images"][texture_index]["uri"];

			// Create the new texture.
			aladdin_3d::Texture new_texture = aladdin_3d::Texture((file_dir_path + texture_uri).c_str(), material_name.c_str(), textures.size());

			// Add it to the textures.
			textures.push_back(new_texture);

		}

		return textures;

	}

	std::vector<aladdin_3d::Vertex> LoaderGLTF::assembleVertices
	(
		std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> texUVs
	)
	{
		std::vector<aladdin_3d::Vertex> vertices;
		for (size_t i = 0; i < positions.size(); i++)
		{
			vertices.push_back
			(
				aladdin_3d::Vertex
				{
					positions[i],
					normals[i],
					glm::vec3(1.0f, 1.0f, 1.0f),
					texUVs[i]
				}
			);
		}
		return vertices;
	}

	std::vector<glm::vec2> LoaderGLTF::groupFloatsVec2(std::vector<float> floatVec)
	{
		std::vector<glm::vec2> vectors;
		for (size_t i = 0; i < floatVec.size(); i) {
            
            // Do this for GLFW.
            glm::vec2 coord(floatVec[i++], floatVec[i++]);
            coord = glm::vec2(coord.y, coord.x);
            
			vectors.push_back(coord);
            
		}
		return vectors;
	}

	std::vector<glm::vec3> LoaderGLTF::groupFloatsVec3(std::vector<float> floatVec)
	{
		std::vector<glm::vec3> vectors;
		for (size_t i = 0; i < floatVec.size(); i)
		{
			vectors.push_back(glm::vec3(floatVec[i++], floatVec[i++], floatVec[i++]));
		}
		return vectors;
	}

	std::vector<glm::vec4> LoaderGLTF::groupFloatsVec4(std::vector<float> floatVec)
	{
		std::vector<glm::vec4> vectors;
		for (size_t i = 0; i < floatVec.size(); i)
		{
			vectors.push_back(glm::vec4(floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]));
		}
		return vectors;
	}

	void LoaderGLTF::getGeometries(std::vector<Geometry> *geoms, std::vector<glm::mat4> *matrices) {

		(*geoms) = this->geometries;
		(*matrices) = this->transform_matrixes;

	}

}
