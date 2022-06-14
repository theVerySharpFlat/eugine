//
// Created by aiden on 6/14/22.
//

#ifndef EUGINE_VKRENDERPASS_H
#define EUGINE_VKRENDERPASS_H

#include <volk.h>

namespace eg::rendering::VKWrapper {

    class VkDevice;
    class VkWindow;

    class VkRenderPass {
    public:
        VkRenderPass(VkDevice& device, VkWindow& window);
        ~VkRenderPass();

        void init();
        void destruct();

    private:
        ::VkRenderPass m_renderPass;

        VkDevice& m_device;
        VkWindow& m_window;
    };
}

#endif //EUGINE_VKRENDERPASS_H
