#ifndef EG_RENDERING_VERTEX_BUFFER_LAYOUT
#define EG_RENDERING_VERTEX_BUFFER_LAYOUT

#include "Types.h"
#include <initializer_list>

namespace eg::rendering {

    struct VertexAttrib {
        ShaderType type; 
        u32 count = 0;
    };

    class VertexBufferLayout {
    public:
        VertexBufferLayout(u32 numAttributes);
        VertexBufferLayout(std::initializer_list<VertexAttrib> attributes);
        
        void setAttribute(u32 index, VertexAttrib attrib);

        u32 getStride() const;

        const std::vector<VertexAttrib>& getAttributes() const {return m_attributes;};

    private:
        u32 m_numAttributes;

        std::vector<VertexAttrib> m_attributes;
    };
    
}

#endif