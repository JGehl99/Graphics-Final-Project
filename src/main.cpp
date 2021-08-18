// Use Static Library instead of DLL
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Texture.h"
#include "Window.h"
#include "Camera.h"
#include "Mesh.h"
#include "LightMesh.h"
#include "ClockMesh.h"


// Name: Joshua Gehl
//   ID: 105025519


// Width, height
constexpr GLint WIDTH = 1600, HEIGHT = 900;

// Paths to .obj files
std::string boxPath{ "./objects/box.obj" };
std::string cBoxPath{ "./objects/cBox.obj" };
std::string tablePath{ "./objects/table.obj" };
std::string chairPath{"./objects/chair.obj"};
std::string clPath{ "./objects/ceilingLight.obj" };
std::string shrekPath{ "./objects/shrek.obj" };
std::string globePath{ "./objects/globe.obj" };
std::string mugPath{ "./objects/mug.obj" };
std::string phonePath{ "./objects/phone.obj" };
std::string rgbLightPath{ "./objects/rgbLight.obj" };
std::string clockPath{ "./objects/clock.obj" };


// Vector to hold Model ptrs, easier to loop over than to do each one individually
std::vector<Mesh*> meshes;
std::vector<LightMesh*> lMeshes;

// List of Light* to hold Light objects from LightModels
std::vector<Light*> lSources;

// Window
// Width, Height
Window w{ WIDTH , HEIGHT };

// Camera
// Position, Target, Up, FOV, AspectRatio
Camera c{glm::vec3(0.070476, 4.299999, 3.724034), glm::vec3(0.0f, 0.0, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0, (double)WIDTH/(double)HEIGHT};

// Shader
// VertexShaderPath, FragmentShaderPath
Shader s{ "./shaders/vertexShader.glsl", "./shaders/fragmentShader.glsl" };
Shader ls{"./shaders/lsVertexShader.glsl", "./shaders/lsFragmentShader.glsl"};

// Texture
// TexturePath
Texture cBoxTex{ "./textures/cBox.png" };
Texture woodTex{ "./textures/tableWood.png" };
Texture chairTex{ "./textures/chair.png" };
Texture whiteTex{ "./textures/white.jpg" };
Texture blackTex{ "./textures/black.png" };
Texture floorTex{ "./textures/floorTile.jpg" };
Texture ceilingTex{ "./textures/ceiling.png" };
Texture shrekTex{ "./textures/shrek.png" };
Texture globeTex{"./textures/globe.png"};
Texture mugTex{ "./textures/mug.png" };
Texture phoneTex{ "./textures/phone.png" };
Texture brickTex{ "./textures/brick.jpeg" };
Texture clockTex{ "./textures/clock.png" };

// Mesh
// Tex, Shader, Camera, OBJPath, LightSources         

Mesh floorMesh{ floorTex, s, c, boxPath, &lSources };
Mesh ceiling{ ceilingTex, s, c, boxPath, &lSources };
Mesh walls{ brickTex, s, c, boxPath, &lSources };

Mesh table{ woodTex, s, c, tablePath, &lSources };

Mesh chair1{ chairTex, s, c, chairPath, &lSources };
Mesh chair2{ chairTex, s, c, chairPath, &lSources };
Mesh chair3{ chairTex, s, c, chairPath, &lSources };
Mesh chair4{ chairTex, s, c, chairPath, &lSources };

Mesh shrek{ shrekTex, s, c, shrekPath, &lSources};

Mesh cardboardBox{ cBoxTex, s, c, cBoxPath, &lSources };

Mesh globe{globeTex, s, c, globePath, &lSources};

Mesh mug{ mugTex, s, c, mugPath, &lSources };

Mesh secondHand{blackTex, s, c, boxPath, &lSources};
Mesh minuteHand{ blackTex, s, c, boxPath, &lSources };
Mesh hourHand{ blackTex, s, c, boxPath, &lSources };


// ClockMesh
// Tex, Shader, Camera, OBJPath, LightSources, Seconds Hand, Minutes Hand, Hours Hand
ClockMesh clockModel{ clockTex, s, c, clockPath, &lSources, secondHand, minuteHand, hourHand };


// LightMesh
// Tex, Shader, lightShader, Camera, OBJPath, LightSources, LightColor, aStr, dStr, sStr, constant, linear, quadratic
LightMesh ceilingLightMesh{ whiteTex, s, ls, c, clPath, &lSources, glm::vec3(1.0f, 1.0f, 1.0f), 0.25, 1.0, 0.25, 1.0, 0.045, 0.0075 };
LightMesh phone{ phoneTex, s, ls, c, phonePath, &lSources, glm::vec3(1.0f, 1.0f, 1.0f), 0.05, 1.0, 0.1, 1.0, 0.35, 0.44 };
LightMesh rgbLight{ whiteTex, s, ls, c, rgbLightPath, &lSources, glm::vec3(1.0f, 1.0f, 1.0f), 0.1, 1.0, 0.7, 1.0, 0.1, 0.05 };

int main() {

    // Add Mesh ptrs to meshes vector
    
    meshes.push_back(&floorMesh);
    meshes.push_back(&ceiling); 
    meshes.push_back(&walls);

    meshes.push_back(&table);

    meshes.push_back(&chair1);
    meshes.push_back(&chair2);
    meshes.push_back(&chair3);
    meshes.push_back(&chair4);

    meshes.push_back(&shrek);

    meshes.push_back(&cardboardBox);
    meshes.push_back(&mug);
    meshes.push_back(&globe);

    meshes.push_back(&clockModel);

    meshes.push_back(&secondHand);
    meshes.push_back(&minuteHand);
    meshes.push_back(&hourHand);

    // Add LightMesh ptrs to lMeshes vector
    lMeshes.push_back(&ceilingLightMesh);
    lMeshes.push_back(&phone);
    lMeshes.push_back(&rgbLight);

    // Add Light from LightMesh to lSources vector
    for (auto lm : lMeshes) {
        lSources.push_back(&(*lm).getLightSource());
    }


    // Transform Meshes to where they need to be
    floorMesh.translate(glm::vec3(0.0f, -2.0f, 0.0f));
    floorMesh.scale(glm::vec3(13.0f, 0.05f, 13.0f));

    ceiling.translate(glm::vec3(0.0f, 11.2f, 0.0f));
    ceiling.scale(glm::vec3(13.0f, 0.05f, 13.0f));

    walls.scale(glm::vec3(12.0f, 12.0f, 12.0f));
    
    table.scale(glm::vec3(0.55f, 0.55f, 0.55f));
    table.translate(glm::vec3(0.0f, -2.0f, 0.0f));

    chair1.translate(glm::vec3( 3.0f, -2.0f, 0.0f));
    chair1.rotate(-90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    chair1.scale(glm::vec3(0.75f, 0.75f, 0.75f));

    chair2.translate(glm::vec3( 0.0f, -2.0f, 3.0f));
    chair2.rotate(-180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    chair2.scale(glm::vec3(0.75f, 0.75f, 0.75f));

    chair3.translate(glm::vec3(-3.0f, -2.0f, 0.0f));
    chair3.rotate(-270.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    chair3.scale(glm::vec3(0.75f, 0.75f, 0.75f));

    chair4.translate(glm::vec3( 0.0f, -2.0f,-3.0f));
    chair4.scale(glm::vec3(0.75f, 0.75f, 0.75f));

    shrek.translate(glm::vec3(0.0f, 1.0f, -3.0f));

    cardboardBox.translate(glm::vec3(0.0f, 2.1f, 0.0f));
    cardboardBox.scale(glm::vec3(1.1f, 1.1f, 1.1f));

    mug.translate(glm::vec3(1.0f, 2.1f, 0.75f));
    mug.rotate(45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    mug.scale(glm::vec3(5.0f, 5.0f, 5.0f));

    globe.scale(glm::vec3(0.03f, 0.03f, 0.03f));
    globe.translate(glm::vec3(-0.75f, 2.05f, 0.25f));


    // Transform ClockMeshes to where they need to be (this also transforms the clock hands)
    clockModel.translate(glm::vec3(-12.0f, 5.0f, 0.0f));
    clockModel.rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));


    // Transform light Sources to where they need to be
    ceilingLightMesh.translate(glm::vec3(0.0f, 10.0f, 0.0f));

    phone.translate(glm::vec3(0.5f, 2.15f, -0.25f));
    phone.rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    phone.rotate(-30.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    phone.scale(glm::vec3(2.5f, 2.5f, 2.5f));

    rgbLight.rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    rgbLight.rotate(-90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    rgbLight.translate(glm::vec3(12.2f, 5.0f, 0.0f));
    rgbLight.scale(glm::vec3(0.75f, 0.75f, 0.75f));

    // Angle passed to LightMesh.cycleColor() function
    float rgbLightAngle = 0.0f;

    //Initialize the time on the clock to current time
    clockModel.initTime();

    //Window loop
    while (!glfwWindowShouldClose(w.getWindow())) { 

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        // Draw light sources
        for (auto lm : lMeshes) {
            (*lm).render();
        }

        // Draw Models
        for (auto m : meshes) {
            (*m).render();
        }

        // Cycle the rgbLight
        rgbLight.cycleColor(rgbLightAngle);
        rgbLightAngle += 3.0f;

        // Update clock time
        clockModel.updateTime(); 

        // Swap buffers after drawing to back buffer
        glfwSwapBuffers(w.getWindow());

        // Process pending events that occured this loop
        glfwPollEvents();

        // Callback for camera controls
        c.camera_callback(w); 

        glFlush();
    }

    // Destroy window and free memory
    glfwTerminate();
    return 0;
}


void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Hit Esc to lock camera and unlock mouse
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        if (w.isPaused()) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        w.togglePause();
    }

    // Toggle Camera Movement
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) { 
        c.toggleMovement(); 
    }

    // Toggle Lights
    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        ceilingLightMesh.toggleLight();
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        phone.toggleLight();
    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
        rgbLight.toggleLight();
    }

    // Change camera positions
    if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
        c.updateView(
            glm::vec3(-8.221084, 8.300014, 8.046157),
            glm::vec3(6.584187, -4.047455, -6.151406),
            glm::vec3(0.000000, 1.000000, 0.000000)
        );
    }
    if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
        c.updateView(
            glm::vec3(-1.533707, 4.650001, 1.077777),
            glm::vec3(6.493767, -6.294962, -3.972169),
            glm::vec3(0.000000, 1.000000, 0.000000)
        );
    }
    if (key == GLFW_KEY_6 && action == GLFW_PRESS) {
        c.updateView(
            glm::vec3(8.460988, 1.000001, -7.783555),
            glm::vec3(-4.568258, 3.185136, 8.158438),
            glm::vec3(0.000000, 1.000000, 0.000000)
        );
    }
    if (key == GLFW_KEY_7 && action == GLFW_PRESS) {
        c.updateView(
            glm::vec3(-9.508314, 4.750000, 1.201404),
            glm::vec3(-8.212931, -0.360060, -5.476588),
            glm::vec3(0.000000, 1.000000, 0.000000)
        );
    }
}