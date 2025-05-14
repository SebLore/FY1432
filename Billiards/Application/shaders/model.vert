#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;      // We'll add normals later
layout (location = 2) in vec2 aTexCoords; // We'll add tex coords later

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// To Fragment Shader
out vec3 FragPos_World; // Vertex position in world space
out vec3 Normal_World;  // Normal in world space

void main() {
    FragPos_World = vec3(model * vec4(aPos, 1.0));
    Normal_World = mat3(transpose(inverse(model))) * aNormal; // Transform normals correctly

    gl_Position = projection * view * vec4(FragPos_World, 1.0);
    // TexCoords_Out = aTexCoords; // If using textures
}