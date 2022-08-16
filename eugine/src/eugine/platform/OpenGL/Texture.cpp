#include "Texture.h"
#include "eugine/util/filesystem.h"
#include "glad/glad.h"
#include "stb_image.h"

#include "error.h"
#include <bits/types/FILE.h>

namespace eg::rendering::GLWrapper {
    Texture::Texture(const fs::path& path) : m_name(path.c_str()) {
        std::string data = filesystem::getFileContents(path);
        GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_ID));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_ID));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

        i32 width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        const unsigned char* img = stbi_load_from_memory((uint8_t*)data.c_str(), data.size(), &width, &height, &nrChannels, 4);

        GLenum imageType = GL_RGBA;

        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, imageType, width, height, 0, imageType, GL_UNSIGNED_BYTE, img));
        GLCall(glGenerateMipmap(GL_TEXTURE_2D));

        stbi_image_free((void*)img);
    }

    Texture::Texture(const u8* data, u32 size, const char* name) : m_name(name) {
        GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_ID));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_ID));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        i32 width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        const unsigned char* img = stbi_load_from_memory(data, size, &width, &height, &nrChannels, 4);

        GLenum imageType = GL_RGBA;

        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, imageType, width, height, 0, imageType, GL_UNSIGNED_BYTE, img));
        GLCall(glGenerateMipmap(GL_TEXTURE_2D));

        stbi_image_free((void*)img);
    }

    void Texture::bind(u32 slot) const {
        GLCall(glActiveTexture(GL_TEXTURE0 + slot))
        GLCall(glBindTexture(GL_TEXTURE_2D, m_ID));
    }

    void Texture::unBind() const {
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    }

    bool Texture::operator==(const ::eg::rendering::Texture &texture) const {
        return m_ID == (((const ::eg::rendering::GLWrapper::Texture&)texture).m_ID);
    }

    Texture::~Texture() {
        GLCall(glDeleteTextures(1, &m_ID));
    }
}
