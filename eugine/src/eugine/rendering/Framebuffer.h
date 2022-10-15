//
// Created by aiden on 10/8/22.
//

#ifndef EUGINE_FRAMEBUFFER_H
#define EUGINE_FRAMEBUFFER_H

namespace eg::rendering {
    class Framebuffer {
    public:
        enum Usage {
            DEFAULT,
            OFFSCREEN_RENDER_TARGET_WITH_READBACK
        };
        virtual ~Framebuffer(){};

        static Ref<Framebuffer> create(Usage usage);
    };
}


#endif //EUGINE_FRAMEBUFFER_H
