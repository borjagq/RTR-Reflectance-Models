#version 330 core

in vec3 vertexPosition; // Position from the VS.
in vec3 vertexNormal;    // Normal from the VS.
in vec3 vertexColor;    // Color from the VS.
in vec2 vertexUV;        //UV coordinates from the VS.

uniform vec4 lightColor;            // Light color.
uniform vec3 lightPos;              // Light position.
uniform vec3 cameraPosition;        // Position of the camera.
uniform float materialShininess;    // Extra shininess.
uniform float shininess;
uniform float albedo;
uniform float roughness;
uniform float lightColorRatio;

const float PI = 3.1415926535897932384626433832795;

vec3 ambientCol = lightColor.rgb * lightColorRatio;
vec3 specularCol = lightColor.rgb;

out vec4 outColor; // Outputs color in RGBA.

float t(float s, vec3 lightDir, vec3 norm, vec3 eye) {
    
    if (s > 0) {
       return max(dot(norm, lightDir), dot(norm, eye));
    } else{
        return 1.0;
    }
    
}

void main() {
    
    // Get the normal ready to use.
    vec3 normal = normalize(vertexNormal);

    // Get the light direction.
    vec3 lightDir = lightPos - vertexPosition;
    
    // Get the view direction.
    vec3 viewDir = cameraPosition - vertexPosition;
        
    // Get the base color from the texture.
    vec4 textureColor = vec4(vertexColor, 1.0);

    float a = (1/PI) * (1.0 - (0.5 * ((roughness * roughness) / ((roughness * roughness) + 0.33))) +
           (0.17 * albedo * ((roughness * roughness) / ((roughness * roughness) + 0.13))));
    float b = (1/PI) * (0.45 * ((roughness * roughness) / ((roughness * roughness) + 0.09)));
    float s = dot(lightDir, viewDir) - (dot(normal, lightDir) * dot(normal, viewDir));
    float lambert = albedo * dot(normal, lightDir) * (a + (b * (s / t(s, lightDir, normal, viewDir))));

    float specularity = 0.0;
    if (lambert > 0) {
        vec3 h = normalize(lightDir+viewDir);
        float specularAngle = max(dot(h, normal), 0.0);
        specularity = pow(specularAngle, shininess);
    }
    
    vec3 fragColour = ambientCol + lambert * textureColor.rgb + specularity * specularCol;

    outColor = vec4(fragColour, 1);

}

