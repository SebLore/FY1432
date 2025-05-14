#version 330 core
out vec4 FragColor;

in vec3 FragPos_World;
in vec3 Normal_World;
// in vec2 TexCoords_Out; // If using textures

// Uniforms for simple lighting
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos_World; // Light position in world space
uniform vec3 viewPos_World;  // Camera position in world space

void main() 
{
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(Normal_World);
    vec3 lightDir = normalize(lightPos_World - FragPos_World);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular (simple Blinn-Phong like)
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos_World - FragPos_World);
    vec3 reflectDir = reflect(-lightDir, norm); // Or use halfway vector for Blinn-Phong
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // 32 is shininess
    vec3 specular = specularStrength * spec * lightColor;
    
    // final result
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
        // FragColor = vec4(objectColor, 1.0); // Or just fixed color for now
}