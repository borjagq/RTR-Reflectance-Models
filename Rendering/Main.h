/**
 * @file Main.h
 * @brief Main header aladdin 3d file.
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

#ifndef ALADDIN_3D_MAIN_H_
#define ALADDIN_3D_MAIN_H_

#define WINDOW_WIDTH 1400
#define WINDOW_HEIGHT 800
#define GAME_NAME "Real-time rendering"
#define NORM_SIZE 1.0

#include <vector>
#include <string>
#include <ctime>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Classes/Camera/Camera.h"
#include "Classes/Object/Object.h"
#include "Classes/Shader/Shader.h"

std::vector<aladdin_3d::Camera> cameras;	/// Holds all the existing cameras.
std::vector<aladdin_3d::Object> objects;	/// Holds all the displayed objects.
std::vector<aladdin_3d::Shader> shaders;	/// Holds all the initialized shanders.
int current_camera = 0;            /// Current camera activated.
int current_object = 0;            /// Current camera activated.
int current_shader = -1;           /// Current camera activated.
GLFWwindow *window = 0;						/// Window ID.
double internal_time = 0;					/// Time that will rule everything in the game.
double time_start = 0;						/// Time that will count as the beginning.
aladdin_3d::Light scene_light;              /// The light in the scene.

// Shader params.
float blinn_shininess = 16.0;
float blinn_lightPower = 5.0;
float blinn_minAmbientLight = 0.25;

float toon_margin = 0.85 * 3.1415926535897932384626433832795 / 2;
int toon_steps = 4;
float toon_lightPower = 5.0;
float toon_minAmbientLight = 0.45;

float oren_shininess = 100.0;
float oren_albedo = 1.1;
float oren_roughness = 0.1;
float oren_ambientColRatio = 0.1;

float cook_F0 = 0.8;
float cook_roughness = 0.1;
float cook_k = 0.2;

const glm::vec4 background(82 / 255.0, 103 / 255.0, 125 / 255.0, 1.0);	/// This is just the fog color.

/**
 * @brief Clean everything to end the program.
 *
 * Clean everything to end the program.
 */
void clean();

/**
 * @brief Display the elements.
 *
 * This function will be called in the main loop to display the elements.
 */
void display();

/**
 * @brief Handles the key events.
 *
 * Handles the freeglut key events.
 */
void handleKeyEvents(unsigned char key, int x, int y);

/**
 * @brief Init the elements of the program
 *
 * Initialize the objects, elements and all.
 */
void initElements();

/**
 * @brief Init the environment.
 *
 * Initialize the OpenGL, Glew and Freeglut environments.
 */
GLFWwindow* initEnvironment(int argc, char** argv);

/**
 * @brief Main function.
 * 
 * Main function.
 */
int main(int argc, char** argv);

#endif  //!ALADDIN_3D_MAIN_H_
