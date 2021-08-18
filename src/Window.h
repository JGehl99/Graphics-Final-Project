#ifndef WINDOW_
#define WINDOW_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


// Callback to adjust Viewport size on framebuffer change
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    std::cout << "New Size: " << width << "x" << height << std::endl;
    glViewport(0, 0, width, height);
}

// Callback for when an error occurs
void error_callback(int code, const char* description) {
    std::cout << "OpenGL Error: " << code << ": " << description << std::endl;
}

// Callback for key presses, using this to check if window is paused
void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


// Class for GLFWwindow
class Window {

private:
    int width;
    int height;
    GLFWwindow* window;
    bool paused = false;

public:

	Window(int w, int h): width(w), height(h){  
         
        // If glfw doesn't initialize, exit
        if (!glfwInit()) {
            std::cout << "GLFW Problem" << std::endl;
            exit(-1);
        }

        //Set version to OpenGL v3.3 and to use modern OpenGL
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

        // Set window NOT resizable
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        // Create window of size WITDH x HEIGHT and check if its created, if not, exit
        window = glfwCreateWindow(width, height, "Final Project", NULL, NULL);
        if (!window) {
            std::cout << "Cannot Create Window; terminating..." << std::endl;
            glfwTerminate();
            exit(-2);
        }

        //Set Callbacks
        glfwSetErrorCallback(error_callback);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetKeyCallback(window, glfw_key_callback);

        // Hide cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); 

        // Set window as current context
        glfwMakeContextCurrent(window);
        // Set clear color to white
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        // Set drawing color - grey
        glColor3f(0.5f, 0.1f, 0.8f);
        // Enables V-Sync
        glfwSwapInterval(1);
        // Enable z-buffer depth test
        glEnable(GL_DEPTH_TEST);

        // Initialize GLEW
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            std::cout << "Cannot Initialize GLEW; terminating..." << std::endl;
            exit(-3);
        }
	}

    // Getter for GLFWwindow* window
    GLFWwindow* getWindow() { return this->window; }

    //Getters and setters for width and height
    int getWidth(){ return width; }
    int getHeight() { return height; }

    void setWidth(int width){
        this->width = width;
        glViewport(0, 0, width, height);
    }

    void setHeight(int height) {
        this->height = height;  
        glViewport(0, 0, width, height); 
    }

    // Toggle paused
    bool togglePause() {
        paused = !paused;
        return paused;
    }

    // Check bool value of paused
    bool isPaused() { return paused; } 
};

#endif 