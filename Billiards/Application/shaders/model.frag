#version 460 core

in vec3 vWorldPos;
in vec2 vUV;
in vec3 vNormal;
in vec3 vViewDir;

// Uniforms for simple lighting
uniform vec3 uObjectColor;

uniform vec3 uLightColor;
uniform vec3 uLightDirection;
uniform float uLightIntensity;

out vec4 finalColor;

void main() 
{
    // just the diffuse for now
    vec3 normal = normalize(vNormal);

    float D = max(0.0, cos(dot(normal, -uLightDirection))); // Lambert diffuse factor

    vec3 diffuse = uLightColor * D * uLightIntensity;

    finalColor = vec4(diffuse, 1.0)
}