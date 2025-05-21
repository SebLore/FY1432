#version 460 core

in vec3 vWorldPos;
in vec2 vTexCoords;
in vec3 vNormal;
in vec3 vViewDir;

// // Uniforms for simple lighting
// uniform vec3 uObjectColor;
// 
// uniform vec3 uLightColor;
// uniform vec3 uLightDirection;
// uniform float uLightIntensity;

out vec4 finalColor;

void main() 
{
    // for now, light is hard-coded
    vec3 uLightColor = vec3(1.0, 1.0, 1.0); // white light
    vec3 uLightDirection = vec3(0.0, 1.0, 0.0); // light direction
    float uLightIntensity = 1.0; // light intensity
    vec3 uObjectColor = vec3(1.0, 0.5, 0.5); // object color

    // just the diffuse for now
    vec3 normal = normalize(vNormal);

    float D = max(0.0, cos(dot(normal, -uLightDirection))); // Lambert diffuse factor

    vec3 diffuse = uLightColor * D * uLightIntensity;

    finalColor = vec4(vWorldPos, 1.0);
    finalColor = vec4(diffuse, 1.0);
}