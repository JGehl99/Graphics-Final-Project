#ifndef LIGHTOBJECTMESH
#define LIGHTOBJECTMESH

#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <functional>
#include "Camera.h"
#include "Light.h"
#include "Mesh.h"

class LightMesh : public Mesh {
private:

    // Holds light shader
    Shader& lightShader;

    // Light color value of 0, 0, 0 for when the light is turned off
    glm::vec3 noLight{ 0.0f, 0.0f, 0.0f };

    // Holds the light color thats being used before turning off the light
    glm::vec3 prevColor{};

    // Holds if light is on or off
    bool isLightOn = true;

    // Light object, holds everything needed to calculate 
    // ambient, diffuse, and specular lighting
    Light ls;


public:

    // LightMesh Constructor
    LightMesh(Texture& tex, Shader& s, Shader& lsh, Camera& c, std::string path, std::vector<Light*>* lsrc, glm::vec3 lc, float aStr, float dStr, float sStr, float constant, float linear, float quadratic)
        : Mesh(tex, s, c, path, lsrc), lightShader(lsh) {

        // Create Light object
        ls = Light(glm::vec3(model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)), lc, aStr, dStr, sStr, constant, linear, quadratic); 
        prevColor = lc;
    }

    void render() {

        // If the light is on, draw Mesh using lsShaders, else use regular shaders
        // Explanation:

        // I have 2 sets of shaders, 1 for objects that give light (lsShader), and 1 that has lighting applied to it (Shader)

        // When the light source is on, it uses the lsShader shaders because it doesnt
        // have any lighting since the Mesh itself is a light source.
        // But, when the light is off, it should have lighting, so use the draw() function
        // from the base class Mesh to draw so it will use the Shader shaders instead

        if (isLightOn) {
            // Use Associated Shader
            lightShader.use();

            // Bind model, view, and proj matricies to shader
            lightShader.setUniformMat4("model", model);
            lightShader.setUniformMat4("view", camera.getView());
            lightShader.setUniformMat4("projection", camera.getProj());
            lightShader.setUniformVec4("lightColor", glm::vec4(ls.lightColor, 1.0f));

            // Bind texture and vao
            glBindTexture(GL_TEXTURE_2D, texture.id);
            glBindVertexArray(vao);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

            // Draw elements
            glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

            // Unbind texture and vao
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0);
        }
        else {
            Mesh::render(); 
        }
    }

    // Update lightPos to new location, always from center of shape
    void updateLightPos() {
        ls.lightPos = (glm::vec3(model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
    }


    // Update light color
    // If light is on, change the light color
    // If light is off, change the prevColor since the
    // light is set to prevColor when its turned back  on
    void updateLightColor(glm::vec3 newColor) {
        if (isLightOn) {
            ls.lightColor = newColor;
        }
        else {
            prevColor = newColor;
        }
    }

    // Toggles lighting on and off
    // When turning light off, store current color and set color to black
    // When turning light on, restore color to previous color
    void toggleLight() {
        if (isLightOn) {
            prevColor = ls.lightColor;
            ls.lightColor = noLight;
        }
        else {
            ls.lightColor = prevColor;
        }
        isLightOn = !isLightOn;
    }

    // Rotate around point by angle around axis
    void rotate(glm::vec3 rotPoint, float angle, glm::vec3 axis) {

        // Create translation and inv translation matrix to center of box
        glm::mat4 tr = glm::translate(glm::mat4(1.0f), rotPoint);
        glm::mat4 trInv = glm::inverse(tr);

        // Rotation to apply to objects in box
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);

        model = tr * rot * trInv * model;
        updateLightPos();
    }

    // Rotate around object origin by angle around axis
    glm::mat4& rotate(float angle, glm::vec3 axis) {
        model = glm::rotate(model, glm::radians(angle), axis);
        updateLightPos();
        return model;
    }

    // Scale object by vec
    glm::mat4& scale(glm::vec3 vec) {
        model = glm::scale(model, vec);
        updateLightPos();
        return model;
    }

    // Translate object by vec
    glm::mat4& translate(glm::vec3 vec) {
        glm::mat4 mat = glm::translate(glm::mat4(1.0f), vec);
        model = mat * model;
        updateLightPos();
        return model;
    }

    // Cycle between rgb
    void cycleColor(float angle) {
        updateLightColor(glm::vec3(
            std::sin(0.3 * glm::radians(std::fmod(angle, 1081)) + 0),
            std::sin(0.3 * glm::radians(std::fmod(angle, 1081)) + 2),
            std::sin(0.3 * glm::radians(std::fmod(angle, 1081)) + 4)
        ));
    }

    // Cycle between on and off
    void cycleStrobe(float angle) {
        updateLightColor(glm::vec3(
            std::sin(0.3 * glm::radians(std::fmod(angle, 720))),
            std::sin(0.3 * glm::radians(std::fmod(angle, 720))),
            std::sin(0.3 * glm::radians(std::fmod(angle, 720)))
        ));
    }

    // Getter and setter for Light
    Light& getLightSource() { return ls; }
    void setLightSource(Light l) { this->ls = l; }
};



#endif