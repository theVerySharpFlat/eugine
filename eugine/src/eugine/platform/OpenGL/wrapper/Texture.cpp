#include "Texture.h"
#include "eugine/util/filesystem.h"
#include "glad/glad.h"
#include "stb_image.h"

#include "error.h"
#include <bits/types/FILE.h>

namespace eg::GLWrapper {
    Texture::Texture(const fs::path& path) {
        std::string data = filesystem::getFileContents(path);
        GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_ID));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_ID));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        i32 width, height, nrChannels;
        const unsigned char* img = stbi_load_from_memory((uint8_t*)data.c_str(), data.size(), &width, &height, &nrChannels, 0);

        GLenum imageType;
        if(nrChannels == 3) {
            imageType = GL_RGB;
        } else if(nrChannels == 4) {
            imageType = GL_RGBA;
        } else {
            EG_ASSERT(false, "unsupported number of channels!");
        }

        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, imageType, width, height, 0, imageType, GL_UNSIGNED_BYTE, img));
        GLCall(glGenerateMipmap(GL_TEXTURE_2D));

        stbi_image_free((void*)img);
    }
    
    void Texture::bind() const {
        GLCall(glBindTexture(GL_TEXTURE_2D, m_ID));
    }

    void Texture::unBind() const {
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    }

    Texture::~Texture() {
        GLCall(glDeleteTextures(1, &m_ID));
    }
}