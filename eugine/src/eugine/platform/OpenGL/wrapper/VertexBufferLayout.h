//
// Created by aiden on 4/23/22.
//

#ifndef EUGINE_VERTEXBUFFERLAYOUT_H
#define EUGINE_VERTEXBUFFERLAYOUT_H

#define GL_BYTE 0x1400
#define GL_UNSIGNED_BYTE 0x1401
#define GL_SHORT 0x1402
#define GL_UNSIGNED_SHORT 0x1403
#define GL_INT 0x1404
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT 0x1406

namespace eg {
    namespace GLWrapper {

        struct VertexAttrib {
            u32 type = 0x0000;
            u32 count = 0;
        };

        u32 getSizeOfType(u32 type);

        class VertexBufferLayout {
        public:
            VertexBufferLayout(u32 numAttributes);

            void setAttribute(u32 index, VertexAttrib attrib);

            u32 getStride() const;

            const std::vector<VertexAttrib>& getAttributes() const {return m_attributes;};

        private:
            const u32 numAttributes;

            std::vector<VertexAttrib> m_attributes;
        };
    }
}

#endif //EUGINE_VERTEXBUFFERLAYOUT_H
