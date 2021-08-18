#version 330 core

in vec4 pos;                // Point's position
in vec2 texPos;             // Texture position
in vec4 normal;             // Normal to point (already normalized)

out vec4 outPixel;

uniform sampler2D tex;
uniform vec4 cameraPos;     // Camera position in world space.
uniform vec4 lightPos;      // Light's position in world space.
uniform vec4 lightColor;    // Light's Color

// Struct to hold the light values
struct Light{
    vec4 lightPos;
    vec4 lightCol;
    
    float aStr;
    float dStr;
    float sStr;
    float constant;
    float linear;
    float quadratic;
};

// Going to have 3 light sources
#define LIGHTS 3
uniform Light l[LIGHTS];

vec4 getLight(Light l, vec4 normal, vec4 cDir, vec4 pos){
    
    // Attenuation
    float d = length(l.lightPos - pos);
    float at = 1.0 / (l.constant + l.linear * d + l.quadratic * (d * d));

    vec4 lightDir = normalize(l.lightPos - pos);    // Light direction vector
    vec4 lightDirRef = reflect(-lightDir, normal);  // Reflection of lightDir about normal of point's face
     
    // Ambient - Overall minimum light
    vec4 ambient = at * l.aStr * l.lightCol;
    
    // Diffuse - Calculate light of object based on the angle 
    // between the light source and the normal of the face
    vec4 diffuse = at * l.dStr * max(dot(lightDir, normal), 0.0) * l.lightCol;
    
    // Specular - Calculate light of object based on the angle between
    // the reflected light source vector about the normal of the object face
    // in relation to where the camera is located
    vec4 specular = at * l.sStr * pow(max(dot(cDir, lightDirRef), 0.0), 32) * l.lightCol;
    
    return (ambient + diffuse + specular);
}


void main(){

    // Camera Direction Vector
    vec4 camDir = normalize(cameraPos - pos);

    // Empty vector to add light values to
    vec4 result = vec4(0.0, 0.0, 0.0, 1.0);

    // Sum all of the light values
    for(int i = 0; i < LIGHTS; i++){
        result += getLight(l[i], normal, camDir, pos);
    }

    // Final value
    outPixel = result * texture(tex, texPos);
}