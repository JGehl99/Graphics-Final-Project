#version 330 core

// Set locations for in variables
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texturePos;

// Out variables being passed to fragmentShader
out vec2 texPos;
out vec4 color;

// Uniform values
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 lightColor;

void main(){
    
    // Calculate point location in world space
    gl_Position = projection * view * model * vec4(position, 1.0);
    
    // Pass out variables to fragmentShader
    texPos = texturePos;
    color = lightColor;
}