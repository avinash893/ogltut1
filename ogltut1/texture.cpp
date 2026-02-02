#include "texture.h"
#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>

Texture::Texture(const std::string& path)
    : m_ID(0)   // IMPORTANT: initialize
{
    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_2D, m_ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    int w, h, ch;
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &ch, 0);

    if (!data)
    {
        std::cout << "Failed to load texture: " << path << std::endl;
        return;
    }

    GLenum format = (ch == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(
        GL_TEXTURE_2D, 0, format,
        w, h, 0,
        format, GL_UNSIGNED_BYTE, data
    );

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

// ✅ THIS FUNCTION WAS MISSING / WRONG
void Texture::Bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_ID);
}
