#version 330 core

in vec3 vertexPosition; // Position from the VS.
in vec3 vertexNormal;    // Normal from the VS.
in vec3 vertexColor;    // Color from the VS.
in vec2 vertexUV;        //UV coordinates from the VS.

uniform vec4 lightColor;            // Light color.
uniform vec3 lightPos;                // Light position.
uniform vec3 cameraPosition;        // Position of the camera.
uniform float materialShininess;    // Whether to use the texture specular color or not.
uniform float F0;
uniform float roughness;
uniform float k;

const float PI = 3.1415926535897932384626433832795;

out vec4 outColor; // Outputs color in RGBA.

vec3 CookTorrance(vec3 materialDiffuseColor, vec3 materialSpecularColor, vec3 normal, vec3 lightDir, vec3 viewDir, vec3 lightColor) {
    
    float NdotL = max(0, dot(normal, lightDir));
    float Rs = 0.0;
    
    if (NdotL > 0) {
        
        vec3 H = normalize(lightDir + viewDir);
        float NdotH = max(0, dot(normal, H));
        float NdotV = max(0, dot(normal, viewDir));
        float VdotH = max(0, dot(lightDir, H));
        
        // Fresnel reflectance
        float F = pow(1.0 - VdotH, 5.0);
        F *= (1.0 - F0);
        F += F0;
        
        // Microfacet distribution by Beckmann
        float m_squared = roughness * roughness;
        float r1 = 1.0 / (4.0 * m_squared * pow(NdotH, 4.0));
        float r2 = (NdotH * NdotH - 1.0) / (m_squared * NdotH * NdotH);
        float D = r1 * exp(r2);
        
        // Geometric shadowing
        float two_NdotH = 2.0 * NdotH;
        float g1 = (two_NdotH * NdotV) / VdotH;
        float g2 = (two_NdotH * NdotL) / VdotH;
        float G = min(1.0, min(g1, g2));
        
        Rs = (F * D * G) / (PI * NdotL * NdotV);
        
    }
    
    lightColor = lightColor * 0.6;
    
    return materialDiffuseColor * lightColor * NdotL + lightColor * materialSpecularColor * NdotL * (k + Rs * (1.0 - k));
    
}

void main() {

    // Get the normal ready to use.
    vec3 normal = normalize(vertexNormal);
    
    // Get the light direction.
    vec3 lightDir = lightPos - vertexPosition;
    
    // Get the sight direction.
    vec3 viewDir = normalize(cameraPosition - vertexPosition);
    
    // Get the base color from the texture.
    vec4 fragmentColor = vec4(vertexColor, 1.0);
    
    vec3 color = CookTorrance(fragmentColor.rgb, lightColor.rgb, normal, lightDir, viewDir, lightColor.rgb);
    
    // Clip colors.
    color.x = (color.x > 1.0) ? 1.0 : color.x;
    color.y = (color.y > 1.0) ? 1.0 : color.y;
    color.z = (color.z > 1.0) ? 1.0 : color.z;

    outColor = vec4(color, 1.0);

}
