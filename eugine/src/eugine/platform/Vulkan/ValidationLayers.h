//
// Created by aiden on 6/1/22.
//

#ifndef EUGINE_VALIDATIONLAYERS_H
#define EUGINE_VALIDATIONLAYERS_H

namespace eg::rendering::VKWrapper {
#ifdef NDEBUG
    const bool enableValidationLayers = false;
    static const u32 validationLayersCount = 0;
    const char** validationLayers = nullptr;

    void confirmValidationLayerSupport() {}
#else
    const bool enableValidationLayers = true;
    static const u32 validationLayersCount = 1;
    extern const char *validationLayers[validationLayersCount];

    void confirmValidationLayerSupport();

#endif
}

#endif //EUGINE_VALIDATIONLAYERS_H
