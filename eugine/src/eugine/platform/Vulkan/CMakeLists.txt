target_sources(eugine PRIVATE
        VKAPI.cpp
        VkDevice.cpp
        ValidationLayers.cpp
        VkWindow.cpp
        VkRenderPass.cpp
        VkShader.cpp
        VkVertexBuffer.cpp
        VkBufferUtil.cpp
        VkIndexBuffer.cpp
        VkTexture.cpp
        VkCommandBufferUtil.cpp
        VkUniformBuffer.cpp
        VkDescriptorSetAllocator.cpp
        VkImguiSystem.cpp
        VkDynamicBufferFrameAllocator.cpp
        VkRenderer2DLowLevel.cpp
        )

if(EG_VK_VALIDATION)
    target_compile_definitions(eugine PRIVATE "EG_VK_VALIDATION")
endif()