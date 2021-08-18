#version 330 core

// In variables from vertexShader
in vec2 texPos;
in vec4 color;

// Out variable for color
out vec4 outPixel;

// Uniform for tex
uniform sampler2D tex;

void main(){

    // Calculate color of point
    outPixel = texture(tex, texPos) * color;
}