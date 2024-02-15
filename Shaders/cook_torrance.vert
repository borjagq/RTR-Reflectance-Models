#version 330 core

layout (location = 0) in vec3 inVertex;	// Vertex.
layout (location = 1) in vec3 inNormal;	// Normal (not necessarily normalized).
layout (location = 2) in vec3 inColor;	// Color (not necessarily normalized).
layout (location = 3) in vec2 inUV;		// UV coordinates.

uniform mat4 Model;			// Imports the model matrix.
uniform mat4 View;			// Imports the View matrix.
uniform mat4 Projection;	// Imports the projection matrix.
uniform mat4 modelView;		// Imports the modelView already multiplied.
uniform mat4 normalMatrix;	// Imports the normal matrix.
uniform vec3 lightPos;		// Light position.
uniform float time;			// Time in seconds.
uniform float velocity;		// Velocity in m/s.

out vec3 vertexNormal;		// Passes the normal to the fragment shader.
out vec3 vertexColor;		// Passes the color to the fragment shader.
out vec2 vertexUV;			// Passes the UV coordinates to the fragment shader.
out vec3 vertexPosition;	// Passes the current vertex to the fragment shader.

void main() {

	// Assigns the direct passes.
	vertexNormal = vec3(normalMatrix * vec4(inNormal, 0.0));
	vertexColor = inColor;
	vertexUV = mat2(0.0, -1.0, 1.0, 0.0) * inUV;

	// Make the MRU calculations.
	float displacement = velocity * time;

	// Get the point after applying Model transformations.
	vec4 transformedPosition = Model * vec4(inVertex, 1.0);

	// Make it move.
	transformedPosition.z -= displacement;

	// Calculates the vertex by applying the transforms to it.
	vec4 tmpVertexPosition = View * transformedPosition;
	vertexPosition = vec3(tmpVertexPosition) / tmpVertexPosition.w;

	// Sets the visualized position by applying the camera matrix.
	gl_Position = Projection * View * transformedPosition;

}
