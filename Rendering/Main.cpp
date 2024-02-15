/**
 * @file main.cpp
 * @brief Main aladdin 3d file.
 * @version 1.0.0 (2022-10-21)
 * @date 2022-10-21
 * @author Borja Garc�a Quiroga <garcaqub@tcd.ie>
 *
 *
 * Copyright (c) Borja Garc�a Quiroga, All Rights Reserved.
 *
 * The information and material provided below was developed as partial
 * requirements for the MSc in Computer Science at Trinity College Dublin,
 * Ireland.
 */

#include "Main.h"

#include <math.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

#include "Classes/Camera/Camera.h"
#include "Classes/Light/Light.h"
#include "Classes/Object/Object.h"
#include "Classes/Shader/Shader.h"
#include "Structs/BoundingBox/BoundingBox.h"

void clean() {

	// Delete all the shaders.
	for (size_t i = 0; i < shaders.size(); i++)
		shaders[i].remove();

}

void display() {
    
    // Init ImGUI for rendering.
    ImGui_ImplGlfwGL3_NewFrame();
    
    // Begin the new widget.
    ImGui::Begin("Parameters");
    
    // Pass these values to the shaders.
    ImGui::Text("Model");
    
    ImGui::RadioButton("Chair", &current_object, 0);
    ImGui::RadioButton("Van", &current_object, 1);
    ImGui::RadioButton("Bunny", &current_object, 2);
    ImGui::RadioButton("Teapot", &current_object, 3);
        
    ImGui::Text("Blinn-Phong");
    
    ImGui::SliderFloat("Shininess", &blinn_shininess, 0.0f, 200.0f);
    ImGui::SliderFloat("Light power", &blinn_lightPower, 0.0f, 100.0f);
    ImGui::SliderFloat("Minimum ambient", &blinn_minAmbientLight, 0.0f, 1.0f);
    
    ImGui::Text("Toon");
    
    ImGui::SliderFloat("Border margin", &toon_margin, 0.0f, 360.0f);
    ImGui::SliderInt("Steps", &toon_steps, 0, 20);
    ImGui::SliderFloat("Light powerT", &toon_lightPower, 0.0f, 100.0f);
    ImGui::SliderFloat("Minimum ambient", &toon_minAmbientLight, 0.0f, 1.0f);
    
    ImGui::Text("Oren-Nayar");
    
    ImGui::SliderFloat("ShininessO", &oren_shininess, 0.0f, 200.0f);
    ImGui::SliderFloat("Albedo", &oren_albedo, 0.0, 5.0);
    ImGui::SliderFloat("Roughness", &oren_roughness, 0.0f, 20.0f);
    ImGui::SliderFloat("Ambient", &oren_ambientColRatio, 0.0f, 1.0f);
    
    ImGui::Text("Cook-Torrance");
    
    ImGui::SliderFloat("F0", &cook_F0, 0.0f, 10.0);
    ImGui::SliderFloat("RoughnessC", &cook_roughness, 0.0, 2.0);
    ImGui::SliderFloat("K", &cook_k, 0.0f, 20.0f);
    
    // Pass these values to the shaders.
    ImGui::Text("Shader");
    
    ImGui::RadioButton("Comparison", &current_shader, -1);
    ImGui::RadioButton("Blinn-Phong", &current_shader, 0);
    ImGui::RadioButton("Toon", &current_shader, 1);
    ImGui::RadioButton("Oren-Nayar", &current_shader, 2);
    ImGui::RadioButton("Cook-Torrance", &current_shader, 3);

    ImGui::End();
    
    // Blinn-Phong.
    shaders[0].activate();
    shaders[0].passFloat("shininess", blinn_shininess);
    shaders[0].passFloat("lightPower", blinn_lightPower);
    shaders[0].passFloat("minAmbientLight", blinn_minAmbientLight);
    
    // Toon.
    shaders[1].activate();
    shaders[1].passFloat("margin", toon_margin);
    shaders[1].passInt("steps", toon_steps);
    shaders[1].passFloat("lightPower", toon_lightPower);
    shaders[1].passFloat("minAmbientLight", toon_minAmbientLight);

    // Oren-Nayar.
    shaders[2].activate();
    shaders[2].passFloat("shininess", oren_shininess);
    shaders[2].passFloat("albedo", oren_albedo);
    shaders[2].passFloat("roughness", oren_roughness);
    shaders[2].passFloat("lightColorRatio", oren_ambientColRatio);
    
    // Cook-Torrance
    shaders[3].activate();
    shaders[3].passFloat("F0", cook_F0);
    shaders[3].passFloat("roughness", cook_roughness);
    shaders[3].passFloat("k", cook_k);
    
    // Specify the color of the background
    glClearColor(background.x, background.y, background.z, background.w);

    // Clean the back buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	// Get the real current time.
	auto current_time = std::chrono::system_clock::now();
	double real_time = std::chrono::duration<double>(current_time.time_since_epoch()).count();

	// Transformthis into the internal time.
	internal_time = real_time - time_start;

	// Get the current camera;
	aladdin_3d::Camera camera = cameras[current_camera];
	
	// Updates and exports the camera matrix to the Vertex Shader
	camera.update();
        
    // Get the current object to be displayed.
    aladdin_3d::Object original_object = objects[current_object];
    
    // Get info from the model.
    aladdin_3d::BoundingBox bb = original_object.getBoundingBox();
    glm::vec3 centre = (bb.min + bb.max) / 2.0f;
    glm::vec3 size = bb.max - bb.min;
    float max_dim = std::max(size.x, std::max(size.y, size.z));
    float scale_rat = NORM_SIZE / max_dim;
    
    if (current_shader == -1) {
    
        // Do the same thing for each model.
        for (int i = 0; i < shaders.size(); i++) {
            
            // Pass variables to the shader.
            shaders[i].activate();
            shaders[i].passFloat("time", internal_time);

            // Copy the originaal object.
            aladdin_3d::Object draw_object = original_object;
            
            // Rotate the object according to time.
            draw_object.rotate(0.0, 1.0, 0.0, std::fmod(internal_time, 30) / 30 * 360);
            
            // Rotate the object to be where it has to be depending on the iteration.
            draw_object.rotate(0.0, 1.0, 0.0, 360 / shaders.size() * i);
            
            // Move so that they do not collide.
            draw_object.translate(0.0, 0.0, 0.7);
            
            // Resize it to normalize it.
            draw_object.scale(scale_rat, scale_rat, scale_rat);
            
            // Center the object and get it in the right position.
            draw_object.translate(-centre.x, -centre.y, -centre.z);
            
            // Draw the current object.
            draw_object.draw(shaders[i], camera);
            
        }
        
    } else {
        
        // Pass variables to the shader.
        shaders[current_shader].activate();
        shaders[current_shader].passFloat("time", internal_time);
        
        // Copy the originaal object.
        aladdin_3d::Object draw_object = original_object;
        
        // Rotate the object according to time.
        draw_object.rotate(0.0, 1.0, 0.0, std::fmod(internal_time, 20) / 20 * 360);
        
        // Resize it to normalize it.
        draw_object.scale(scale_rat, scale_rat, scale_rat);

        // Center the object and get it in the right position.
        draw_object.translate(-centre.x, -centre.y, -centre.z);
        
        // Draw the current object.
        draw_object.draw(shaders[current_shader], camera);
        
    }
    
    // Render ImGUI.
    ImGui::Render();
    
}

void handleKeyEvents(GLFWwindow* window, int key, int scancode, int action, int mods) {
    
    std::cerr << "HELLO HERE" << std::endl;
    
    // Next shader.
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        
        current_shader++;
        if (current_shader >= shaders.size()) current_shader = -1;
                
    }
    
    // Next object.
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        current_object = (current_object + 1) % objects.size();
    
    // Prev object.
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        current_object = (current_object > 0) ? current_object - 1 : (int) objects.size() - 1;

	// Key W will move camera 0 forward.
    if (key == GLFW_KEY_W) {

		// Check if camera 0 is the chosen camera.
		if (current_camera != 0) return;

		// Move forward.
		cameras[0].moveFront();
		
	}

	// Key S will move camera 0 backwards.
    if (key == GLFW_KEY_S) {

		// Check if camera 0 is the chosen camera.
		if (current_camera != 0) return;

		// Move backwards.
		cameras[0].moveBack();

	}

	// Key D will move camera 0 to the right.
    if (key == GLFW_KEY_D) {

		// Check if camera 0 is the chosen camera.
		if (current_camera != 0) return;

		// Move right.
		cameras[0].moveRight();

	}

	// Key A will move camera 0 to the right.
    if (key == GLFW_KEY_A) {

		// Check if camera 0 is the chosen camera.
		if (current_camera != 0) return;

		// Move to the left.
		cameras[0].moveLeft();

	}

	// Key R will move camera up.
    if (key == GLFW_KEY_R) {

		// Check if camera 0 is the chosen camera.
		if (current_camera != 0) return;

		// Move up.
		cameras[0].moveUp();

	}

	// Key F will move camera down.
    if (key == GLFW_KEY_F) {

		// Check if camera 0 is the chosen camera.
		if (current_camera != 0) return;

		// Move up.
		cameras[0].moveDown();

	}

	// Key E will rotate camera left.
    if (key == GLFW_KEY_E) {

		// Check if camera 0 is the chosen camera.
		if (current_camera != 0) return;

		// Move up.
		cameras[0].rotateLeft();

	}

	// Key Q will rotate the camera right.
    if (key == GLFW_KEY_Q) {

		// Check if camera 0 is the chosen camera.
		if (current_camera != 0) return;

		// Move up.
		cameras[0].rotateRight();

	}

	// Key T will bring the camera up.
    if (key == GLFW_KEY_T) {

		// Check if camera 0 is the chosen camera.
		if (current_camera != 0) return;

		// Move up.
		cameras[0].rotateUp();

	}

	// Key G will bring the camera down.
    if (key == GLFW_KEY_G) {

		// Check if camera 0 is the chosen camera.
		if (current_camera != 0) return;

		// Move up.
		cameras[0].rotateDown();

	}

}

void initElements() {

	// Create a white light in the center of the world.
	glm::vec3 light_pos = glm::vec3(2.0f, 3.0f, 2.0f);
	glm::vec4 light_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    scene_light = aladdin_3d::Light(light_pos, light_color);

	// Get the shaders.
	aladdin_3d::Shader blinn_phong("blinn_phong.vert", "blinn_phong.frag");

	// Pass the light to the shader.
    blinn_phong.activate();
    blinn_phong.passLight(scene_light);

    shaders.push_back(blinn_phong);
    
    // Get the shaders.
    aladdin_3d::Shader gooch("toon.vert", "toon.frag");

    // Pass the light to the shader.
    gooch.activate();
    gooch.passLight(scene_light);

    shaders.push_back(gooch);
    
    // Get the shaders.
    aladdin_3d::Shader oren_nayar("oren_nayar.vert", "oren_nayar.frag");

    // Pass the light to the shader.
    oren_nayar.activate();
    oren_nayar.passLight(scene_light);

    shaders.push_back(oren_nayar);
    
    // Get the shaders.
    aladdin_3d::Shader cook_torrance("cook_torrance.vert", "cook_torrance.frag");

    // Pass the light to the shader.
    cook_torrance.activate();
    cook_torrance.passLight(scene_light);

    shaders.push_back(cook_torrance);

	// Creates the first camera object
	aladdin_3d::Camera camera(glm::vec3(0.0f, 0.5f, 2.5f), glm::vec3(0.0f, -0.25f, -1.0f), 45.0f, 0.1f, 300.0f, WINDOW_WIDTH, WINDOW_HEIGHT);
	cameras.push_back(camera);
    
    // Load the objects.
    aladdin_3d::Object eames("eames_chair.fbx", "Assimp");
    objects.push_back(eames);
    
    aladdin_3d::Object volkswagen("volkswagen.fbx", "Assimp");
    objects.push_back(volkswagen);

	aladdin_3d::Object bunny("bunny.fbx", "Assimp");
    bunny.setShininess(75.0);
    objects.push_back(bunny);
    
    aladdin_3d::Object teapot("teapot.obj", "Assimp");
    teapot.setShininess(100.0);
    objects.push_back(teapot);
    
}

GLFWwindow* initEnvironment(int argc, char** argv) {
    
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        
        std::cerr << "ERROR: could not start GLFW3" << std::endl;
        exit(1);

    }
    
    // MacOS initializations.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create the window.
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_NAME, NULL, NULL);
    if (!window) {
        
        std::cerr << "Error 121-1001 - Failed to create the window." << std::endl;
        glfwTerminate();
        exit(1);

    }
    glfwMakeContextCurrent(window);
    
    // Tell what function handles the events.
    glfwSetKeyCallback(window, handleKeyEvents);
    
    // Initialize GLEW and OpenGL.
    GLenum res = glewInit();

    // Check for any errors.
    if (res != GLEW_OK) {

        std::cerr << "Error 121-1002 - GLEW could not be initialized:" << glewGetErrorString(res) << std::endl;
        exit(1);

    }
    
    // Get version info.
    std::cerr << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cerr << "OpenGL version supported " << glGetString(GL_VERSION) << std::endl;
    std::cerr << "GLSL version supported " << (char *) glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    
    // Setup ImGui binding
    ImGui_ImplGlfwGL3_Init(window, true);

    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    
    return window;

}

int main(int argc, char** argv) {

	// Initialise the environment.
    GLFWwindow *window = initEnvironment(argc, argv);
    
	// Initialise the objects and elements.
	initElements();

	// Init the initial time.
	auto current_time = std::chrono::system_clock::now();
	time_start = std::chrono::duration<double>(current_time.time_since_epoch()).count();

	// Main loop.
    while(!glfwWindowShouldClose(window)) {
        
        // Make the things to print everything.
        display();
        
        // Update other events like input handling.
        glfwPollEvents();
        
        glfwSwapBuffers(window);
        
    }

	// Clean everything and terminate.
	clean();
    
    // Terminate ImGUI.
    ImGui_ImplGlfwGL3_Shutdown();
    
    // Close GL context and any other GLFW resources.
    glfwTerminate();

	return 0;

}
