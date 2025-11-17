#pragma once
#include <GL/glew.h>
#include <string>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Texture {
private:
    unsigned int m_ID = 0;
    int m_width = 0, m_height = 0, m_channels = 0;
public:
    Texture() {
        glGenTextures(1, &m_ID);
        glBindTexture(GL_TEXTURE_2D, m_ID);
        unsigned char white[4] = { 255, 255, 255, 255 };
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, white);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    bool loadFromFile(const std::string& path, bool flipVertically = true) {
        stbi_set_flip_vertically_on_load(flipVertically);
        unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);
        if (!data) {
            std::cerr << "failed to load texture: " << path << std::endl;
            return false;
        }
        GLenum format = GL_RGB;
        if (m_channels == 1) format = GL_RED;
        else if (m_channels == 3) format = GL_RGB;
        else if (m_channels == 4) format = GL_RGBA;
        if (m_ID == 0) glGenTextures(1, &m_ID);
        glBindTexture(GL_TEXTURE_2D, m_ID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0);
        return true;
    }

    void bind(int slot = 0) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_ID);
    }

    void unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    unsigned int getID() const { return m_ID; }
};
