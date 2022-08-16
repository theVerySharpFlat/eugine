//
// Created by aiden on 6/1/22.
//

#ifndef EUGINE_VALIDATIONLAYERS_H
#define EUGINE_VALIDATIONLAYERS_H

namespace eg::rendering::VKWrapper {

#ifndef EG_VK_VALIDATION
    const bool enableValidationLayers = false;
    static const u32 validationLayersCount = 0;
    extern const char** validationLayers;

#else
    const bool enableValidationLayers = true;
    static const u32 validationLayersCount = 1;
    extern const char *validationLayers[validationLayersCount];


#endif
    void confirmValidationLayerSupport();
}

#endif //EUGINE_VALIDATIONLAYERS_H
