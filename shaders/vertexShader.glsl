#version 330 core

layout (location = 0) in vec3 position;     // Position
layout (location = 1) in vec2 texturePos;   // Texture
layout (location = 2) in vec3 normalPos;    // Normal

out vec4 pos;       // Position passed to fragmentShader
out vec2 texPos;    // Texture passed to fragmentShader
out vec4 normal;    // Normal passed to fragmentShader

// Uniform mats from Mesh::render()
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normMat;

void main(){
    gl_Position = projection * view * model * vec4(position, 1.0);  // Calculate final position

    // Pass world coordinate points to fragmentShader
    pos = model * vec4(position, 1.0);
    
    // Pass texture pos to fragmentShader
    texPos = texturePos;

    // Pass normals multiplied by normMat to fragmentShader
    normal = normalize(vec4(normMat * normalPos, 0.0));
}