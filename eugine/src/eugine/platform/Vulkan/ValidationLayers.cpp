//
// Created by aiden on 6/1/22.
//

#include "ValidationLayers.h"

#include <volk.h>

namespace eg::rendering::VKWrapper {

    const char* validationLayers[validationLayersCount] = {
        "VK_LAYER_KHRONOS_validation"
    };

    void confirmValidationLayerSupport() {
        u32 layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        auto layers = (VkLayerProperties *) alloca(sizeof(VkLayerProperties) * layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, layers);
        for (auto& validationLayer : validationLayers) {
            bool layerFound = false;

            for (int j = 0; j < layerCount; j++) {

                if (strcmp(layers[j].layerName, validationLayer) == 0) {
                    layerFound = true;
                    break;
                }
            }
            EG_ASSERT(layerFound, "layer \"{}\" not found", validationLayer);
        }
    }
}