#ifndef OBJECTTEXTURE
#define OBJECTTEXTURE

#include <string>
#include <GL/glew.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


// Class for Texture
class Texture {
public:
    unsigned int id{};          // Holds texture ID

    Texture(std::string texPath) {

        // Generate texture
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        // Setting texture attribs

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        // Image information
        int width, height, bytesperpixel;
        unsigned char* data = stbi_load(texPath.c_str(), &width, &height, &bytesperpixel, 0);

        // Check format of image and set the value accordingly
        GLenum format{};
        if (bytesperpixel == 1)
            format = GL_RED;
        else if (bytesperpixel == 3)
            format = GL_RGB;
        else if (bytesperpixel == 4)
            format = GL_RGBA;

        // Create texutre if data is not null
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data); 
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cout << "Failed to load texture" << std::endl;
        }

        // free data and unbind texture
        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};


#endif 
