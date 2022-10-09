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
        enum Usage {
            DEFAULT = 0,
            OFFSCREEN_FOR_READBACK
        };
        VkRenderPass(VkDevice& device, VkWindow& window);
        ~VkRenderPass();

        void init(Usage usage = DEFAULT);
        void destruct();

        ::VkRenderPass getRenderPass() { return m_renderPass; }

    private:
        friend class VKAPI;
        friend class VkShader;
        friend class VkWindow;

        bool m_initSuccess = true;

        ::VkRenderPass m_renderPass = VK_NULL_HANDLE;

        VkDevice& m_device;
        VkWindow& m_window;

        Usage m_usage = DEFAULT;
    };
}

#endif //EUGINE_VKRENDERPASS_H
