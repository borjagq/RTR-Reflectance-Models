#version 330 core

in vec3 vertexPosition; // Position from the VS.
in vec3 vertexNormal;	// Normal from the VS.
in vec3 vertexColor;	// Color from the VS.
in vec2 vertexUV;		//UV coordinates from the VS.

uniform vec4 lightColor;		    // Light color.
uniform vec3 lightPos;			    // Light position.
uniform vec3 cameraPosition;	    // Position of the camera.
uniform float materialShininess;    // Shininess extra.
uniform float margin;
uniform int steps;
uniform float lightPower;
uniform float minAmbientLight;

const float PI = 3.1415926535897932384626433832795;

out vec4 outColor; // Outputs color in RGBA.

vec4 interpolate(vec4 a, vec4 b, float step) {

    // Interpolate each.
    float x = a.x + (b.x - a.x) * step;
    float y = a.y + (b.y - a.y) * step;
    float z = a.z + (b.z - a.z) * step;

    return vec4(x, y, z, 1.0);

}

void main() {

    // Get the normal ready to use.
    vec3 normal = normalize(vertexNormal);
    
    // Get the intensity based on the angle between me and object.
    float intensity = dot(vec3(lightPos), normal);
    
    // Get the base color from the texture.
    vec4 fragmentColor = vec4(vertexColor, 1.0);
    
    // Get the minimum color.
    vec3 minColor = vec3(fragmentColor) * minAmbientLight;
    
    // Modify intensity.
    if (intensity > 1.0) intensity = 1.0;
    if (intensity < 0.0) intensity = 0.0;
    intensity = round(intensity * steps) / steps;

    // Final color.
    vec4 color = interpolate(vec4(minColor, 1.0), fragmentColor, intensity);
        
    // Get the vector from the observer to the fragment.
    vec3 viewVector = normalize(cameraPosition - vertexPosition);
    float angle = acos(dot(viewVector, normal));
    
    // If the angle is too steep, turn it black.
    if (0 + margin < angle && angle < PI - margin)
        color = vec4(0.0, 0.0, 0.0, 1.0);
        
    outColor = color;

}
