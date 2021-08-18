#ifndef OBJECTMESH
#define OBJECTMESH

#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <tiny_obj_loader.h> 
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Texture.h" 
#include "Shader.h"
#include "Camera.h"
#include "Light.h"


// Class for Mesh, this can hold any object to draw to screen,
// currently only used to hold cubes base on verticies and elements in main
class Mesh {
protected:

    // ebo, vao, and vbo
    unsigned int ebo{};
    unsigned int vao{};
    unsigned int vbo{};

    // Holds number of verticies (equal to # of elements in the vector elements)
    int size{};

    // Ref to Texture, Shader, and Camera objects
    Texture& texture;
    Shader& shader;
    Camera& camera;

    // Mesh Matrix and Normal Matrix for this object
    glm::mat4 model;
    glm::mat3 normMat;

    // Ref to verticies, elements, and lSources vector arrays
    std::vector<GLfloat> verticies;
    std::vector<GLuint> elements;
    std::vector<Light*>* lSources;

    void setup() {
        // Creating and binding vao
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Creating, generating, binding, and buffering vertex buffer object
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(GLfloat), verticies.data(), GL_STATIC_DRAW);

        // Creating, generating, binding, and buffering element buffer object
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(GLuint), elements.data(), GL_STATIC_DRAW);

        // Setting up attributes

        //position
        glVertexAttribPointer(
            0,                                      //attribute of vertex shader
            3,                                      //number of elements per vertex
            GL_FLOAT,                               //data type of each element
            GL_FALSE,                               //take values as is
            8 * sizeof(GLfloat),                    //next point appears every 5 floats
            0                                       //offset of the first element
        );
        glEnableVertexAttribArray(0);

        //texturePos
        glVertexAttribPointer(
            1,                                      //attribute of vertex shader
            2,                                      //number of elements per vertex
            GL_FLOAT,                               //data type of each element
            GL_FALSE,                               //take values as is
            8 * sizeof(GLfloat),                    //next point appears every 5 floats
            (void*)(3 * sizeof(GLfloat))            //offset of the first element
        );
        glEnableVertexAttribArray(1);

        //normalPos
        glVertexAttribPointer(
            2,                                      //attribute of vertex shader
            3,                                      //number of elements per vertex
            GL_FLOAT,                               //data type of each element
            GL_FALSE,                               //take values as is
            8 * sizeof(GLfloat),                    //next point appears every 5 floats
            (void*)(5 * sizeof(GLfloat))            //offset of the first element
        );
        glEnableVertexAttribArray(2);

        // Unbind vao
        glBindVertexArray(0);
    }

public:

    Mesh() = default;

    // Take in texture, shader, verticies, and elements
    Mesh(Texture& tex, Shader& s, Camera& c, std::string path, std::vector<Light*>* ls)
        : texture(tex), shader(s), camera(c), lSources(ls){

        if (!loadObject(path, verticies, elements)) {
            std::cout << "Error loading Mesh. Make sure meshes are at ./objects/<model>.obj relative to \"Mesh.h\"" << std::endl;
            exit(-1);
        }

        // Set size
        size = elements.size();

        // Initialize model to identity
        model = glm::mat4(1.0f);
        normMat = glm::mat3(glm::transpose(glm::inverse(model)));

        setup();
    }

    void render() {

        // Use Associated Shader
        shader.use();

        // Loop over light sources and pass the values to the shaders
        for (int i = 0; i < lSources->size(); i++) {
            Light* ls = lSources->at(i);
            shader.setUniformVec4("l[" + std::to_string(i) + "].lightPos", glm::vec4(ls->lightPos, 1.0f));
            shader.setUniformVec4("l[" + std::to_string(i) + "].lightCol", glm::vec4(ls->lightColor, 1.0f));
            shader.setUniformFloat("l[" + std::to_string(i) + "].aStr", ls->aStr);
            shader.setUniformFloat("l[" + std::to_string(i) + "].dStr", ls->dStr);
            shader.setUniformFloat("l[" + std::to_string(i) + "].sStr", ls->sStr);
            shader.setUniformFloat("l[" + std::to_string(i) + "].constant", ls->constant);
            shader.setUniformFloat("l[" + std::to_string(i) + "].linear", ls->linear);
            shader.setUniformFloat("l[" + std::to_string(i) + "].quadratic", ls->quadratic);
        }

        // Update normMat
        normMat = glm::mat3(glm::transpose(glm::inverse(model)));

        // Bind model, view, and proj matricies to shader
        shader.setUniformMat4("model", model); 
        shader.setUniformMat4("view", camera.getView());
        shader.setUniformMat4("projection", camera.getProj());
        shader.setUniformVec4("cameraPos", glm::vec4(camera.getPos(), 1.0f));
        shader.setUniformMat3("normMat", normMat);

        // Bind texture and vao
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.id);
        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        // Draw elements
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

        // Unbind texture and vao
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
    }

    // Loading in the object from a file using Assimp
    bool loadObject(std::string& path, std::vector<GLfloat>& verticies, std::vector<GLuint>& elements) {

        Assimp::Importer importer;

        // Import the scene from the file
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        // If it failed, return false and escalate the error to the constructor by returning false
        if (!scene) {
            std::cout << importer.GetErrorString() << std::endl;
            return false;
        }

        // Get the first mesh (and only mesh, my program only supports .obj files that have 1 mesh)
        const aiMesh* mesh = scene->mMeshes[0];

        // Fill verticies
        verticies.reserve(mesh->mNumVertices);
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {

            // Get pos, tex, and norm coordinates
            aiVector3D pos = mesh->mVertices[i];
            aiVector3D UVW = mesh->mTextureCoords[0][i];
            aiVector3D norm = mesh->mNormals[i];

            // Insert coordinates into verticies in the order that matches the glAttribPointers
            verticies.push_back((GLfloat)pos.x);
            verticies.push_back((GLfloat)pos.y);
            verticies.push_back((GLfloat)pos.z);
            verticies.push_back((GLfloat)UVW.x);
            verticies.push_back((GLfloat)UVW.y);
            verticies.push_back((GLfloat)norm.x);
            verticies.push_back((GLfloat)norm.y);
            verticies.push_back((GLfloat)norm.z);
        }

        // Fill face indices
        elements.reserve(3 * mesh->mNumFaces);
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {

            // Insert elements into element vector
            elements.push_back(mesh->mFaces[i].mIndices[0]);
            elements.push_back(mesh->mFaces[i].mIndices[1]);
            elements.push_back(mesh->mFaces[i].mIndices[2]);
        }
        return true;
    }

    // Rotate around point by angle around axis
    void rotate(glm::vec3 rotPoint, float angle, glm::vec3 axis) {

        // Create translation and inv translation matrix to center of box
        glm::mat4 tr = glm::translate(glm::mat4(1.0f), rotPoint);
        glm::mat4 trInv = glm::inverse(tr);

        // Rotation to apply to objects in box
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);

        model = tr * rot * trInv * model;
    }

    // Rotate around object origin by angle around axis
    glm::mat4& rotate(float angle, glm::vec3 axis) {
        model = glm::rotate(model, glm::radians(angle), axis);
        return model;
    }

    // Scale object by vec
    glm::mat4& scale(glm::vec3 vec) {
        model = glm::scale(model, vec);
        return model;
    }

    // Translate object by vec
    glm::mat4& translate(glm::vec3 vec) {
        glm::mat4 mat = glm::translate(glm::mat4(1.0f), vec);
        model = mat * model;
        return model;
    }

    // Setter and Getter for model
    glm::mat4& getMesh() { return model; }
    void setMesh(glm::mat4) { this->model = model; }
};


#endif