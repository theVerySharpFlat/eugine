#ifndef EG_RENDERING_TYPES_H
#define EG_RENDERING_TYPES_H

namespace eg::rendering {
    enum ShaderType {
        SHDR_BOOL = 0,
        SHDR_INT,
        SHDR_UINT,
        SHDR_FLOAT,
        SHDR_DOUBLE,

        SHDR_VEC2,
        SHDR_VEC3,
        SHDR_VEC4,

        SHDR_MAT2,
        SHDR_MAT3,
        SHDR_MAT4
    };

    inline u32 getSizeOfType(ShaderType type) {
        switch(type) {
            case SHDR_BOOL:   return 4;
            case SHDR_INT:    return 4;
            case SHDR_UINT:   return 4;
            case SHDR_FLOAT:  return 4;
            case SHDR_DOUBLE: return 8;

            case SHDR_VEC2:   return 4 * 2;
            case SHDR_VEC3:   return 4 * 3;
            case SHDR_VEC4:   return 4 * 4;

            case SHDR_MAT2:   return 4 * 2 * 2;
            case SHDR_MAT3:   return 4 * 3 * 3;
            case SHDR_MAT4:   return 4 * 4 * 4;
        }
    }
}

#endif