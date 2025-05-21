#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

uniform mat4 uModel; // from model's transform
uniform mat4 uViewProj; // camera's view * projection
uniform vec3 uViewPos;


out vec3 vWorldPos;       // Vertex position in world space
out vec2 vTexCoords; // Texture coordinates
out vec3 vNormal;    // Normal in world space
out vec3 vViewDir;

void main() 
{
    vec4 worldPos = uModel * vec4(aPosition, 1.0f);
    vWorldPos = worldPos.xyz;
    vTexCoords = aTexCoords;
    vNormal = mat3(uModel) * aNormal;
    vViewDir = uViewPos - worldPos.xyz;

    gl_Position =  uViewProj * worldPos;
}