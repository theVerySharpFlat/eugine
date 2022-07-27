//
// Created by aiden on 7/25/22.
//

#ifndef EUGINE_GLWRAPPER_UNIFORMBUFFER_H
#define EUGINE_GLWRAPPER_UNIFORMBUFFER_H

#include "eugine/rendering/UniformBuffer.h"

namespace eg::GLWrapper {
class UniformBuffer : public ::eg::rendering::UniformBuffer{
    public:
        UniformBuffer() {}
        virtual ~UniformBuffer() {}
    };
}


#endif //EUGINE_GLWRAPPER_UNIFORMBUFFER_H
