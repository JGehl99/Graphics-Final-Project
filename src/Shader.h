#ifndef OBJECTSHADER
#define OBJECTSHADER

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Class for shader stuff
class Shader {

public:
    unsigned int id{};
    std::string vShaderPath;
    std::string fShaderPath;
    std::string fShaderStr;
    std::string vShaderStr;
    const GLchar* vShader;
    const GLchar* fShader;

    Shader() = default;

    Shader(const std::string& vs, const std::string& fs) : vShaderPath(vs), fShaderPath(fs) {

        try {
            // Open file and read it to a string, store ptr to string
            std::ifstream vsStream(vShaderPath);
            std::stringstream vsBuf;
            vsBuf << vsStream.rdbuf();
            vShaderStr = vsBuf.str();
            vsStream.close();
            vShader = vShaderStr.c_str();

            // Open file and read it to a string, store ptr to string
            std::ifstream fsStream(fShaderPath);
            std::stringstream fsBuf;
            fsBuf << fsStream.rdbuf();
            fShaderStr = fsBuf.str();
            fsStream.close();
            fShader = fShaderStr.c_str();

        } catch (...) {
            std::cout << "Error reading Shaders. Please check they exist at that location." << std::endl;
            exit(-4);
        }

        unsigned int vert, frag;

        // Creating vertex shader
        vert = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vert, 1, &vShader, NULL);
        glCompileShader(vert);
        checkCompileErrors(vert, "**Vertex**");

        // Creating fragment shader
        frag = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag, 1, &fShader, NULL);
        glCompileShader(frag);
        checkCompileErrors(frag, "**Fragment**");

        // Create program
        id = glCreateProgram();
        glAttachShader(id, vert);
        glAttachShader(id, frag);
        glLinkProgram(id);
        checkCompileErrors(id, "**Program**");

        // Delete the shaders since they're now linked to our program
        glDeleteShader(vert);
        glDeleteShader(frag);
    }

    // Function to call glUseProgram()
    void use() {
        glUseProgram(id);
    }

    // Sets uniform mat3 in shader
    bool setUniformMat3(const std::string name, const glm::mat3& mat) {
        unsigned int loc = glGetUniformLocation(id, name.c_str());
        if (loc != -1) {
            glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
            return true;
        }
        return false;
    }

    // Sets uniform mat4 in shader
    bool setUniformMat4(const std::string name, const glm::mat4& mat) {
        unsigned int loc = glGetUniformLocation(id, name.c_str());
        if (loc != -1) {
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
            return true;
        }
        return false;
    }

    // Sets uniform vec3 in shader
    bool setUniformVec3(const std::string name, const glm::vec3& mat) {
        unsigned int loc = glGetUniformLocation(id, name.c_str());
        if (loc != -1) {
            glUniform3fv(loc, 1, glm::value_ptr(mat));
            return true;
        }
        return false;
    }
    // Sets uniform vec4 in shader
    bool setUniformVec4(const std::string name, const glm::vec4& mat) { 
        unsigned int loc = glGetUniformLocation(id, name.c_str()); 
        if (loc != -1) {
            glUniform4fv(loc, 1, glm::value_ptr(mat));
            return true;
        }
        return false;
    }

    // Sets uniform float in shader
    bool setUniformFloat(const std::string name, const float f) {
        unsigned int loc = glGetUniformLocation(id, name.c_str());
        if (loc != -1) {
            glUniform1f(loc, f);
            return true;
        }
        return false;
    }


private:
    // Error checking 
    void checkCompileErrors(unsigned int shader, std::string type) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "Shader Compilation Error: " << type << "\n" << infoLog << std::endl;
            }
        }
        else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "Program Linking Error: " << type << "\n" << infoLog << std::endl;
            }
        }
    }
};



#endif // !OBJECTSHADER 
