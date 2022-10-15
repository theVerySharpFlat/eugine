//
// Created by aiden on 7/23/22.
//

#include <GLFW/glfw3.h>
#include <memory>
#include "VkImguiSystem.h"
#include "VKAPI.h"
#include "VkCommandBufferUtil.h"

#include "backends/imgui_impl_vulkan.h"
#include "eugine/platform/Vulkan/VkFramebuffer.h"
#include "imgui/backends/imgui_impl_glfw.h"

namespace eg::rendering::VKWrapper {
    VkImguiSystem::VkImguiSystem(VKAPI& api) : m_api(api) {

    }

    VkImguiSystem::~VkImguiSystem() {

    }

    PFN_vkVoidFunction VkImguiSystem::imguiLoadFN(const char* name, void* userData) {
        VkInstance instance = ((VkImguiSystem*) userData)->m_api.m_instance;
        return vkGetInstanceProcAddr(instance, name);
    }

    static void imguiCheckVkResult(VkResult err) {
        if (err == 0)
            return;
        error("Imgui vulkan error: {}", (int) err);
        if (err < 0) EG_ASSERT(false, "Something fatal happened with the imgui vulkan renderer");
    }

    void VkImguiSystem::onSwapchainRecreation() {
        ImGui_ImplVulkan_SetMinImageCount(m_api.m_vkWindow.getMinImageCount());
    }

    void VkImguiSystem::init() {
        /*
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void) io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        io.ConfigDockingWithShift = true;
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
        */

        ImGui_ImplVulkan_LoadFunctions(imguiLoadFN, this);

        {
            VkDescriptorPoolSize poolSizes[] =
                    {
                            {VK_DESCRIPTOR_TYPE_SAMPLER,                1000},
                            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
                            {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,          1000},
                            {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,          1000},
                            {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,   1000},
                            {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,   1000},
                            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         1000},
                            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         1000},
                            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
                            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
                            {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,       1000}
                    };
            VkDescriptorPoolCreateInfo poolCreateInfo = {};
            poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            poolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
            poolCreateInfo.maxSets = 1000 * IM_ARRAYSIZE(poolSizes);
            poolCreateInfo.poolSizeCount = (uint32_t) IM_ARRAYSIZE(poolSizes);
            poolCreateInfo.pPoolSizes = poolSizes;

            if (vkCreateDescriptorPool(m_api.m_device.getDevice(), &poolCreateInfo, nullptr, &m_pool) != VK_SUCCESS) {
                error("failed to create descriptor pool for vulkan renderer's imgui system!!!");
                return;
            }

            ImGui_ImplGlfw_InitForVulkan((GLFWwindow*) m_api.m_window.getNativeWindow(), true);
            ImGui_ImplVulkan_InitInfo initInfo{};
            initInfo.Instance = m_api.m_instance;
            initInfo.PhysicalDevice = m_api.m_device.getPhysicalDevice();
            initInfo.Device = m_api.m_device.getDevice();
            initInfo.QueueFamily = m_api.m_device.getQueueFamilyIndices().graphicsFamily.value();
            initInfo.Queue = m_api.m_device.getGraphicsQueue();
            initInfo.PipelineCache = VK_NULL_HANDLE;
            initInfo.DescriptorPool = m_pool;
            initInfo.Subpass = 0;
            initInfo.MinImageCount = m_api.m_vkWindow.getMinImageCount();
            initInfo.ImageCount = m_api.m_vkWindow.getImageCount();
            initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
            initInfo.Allocator = nullptr;
            initInfo.CheckVkResultFn = imguiCheckVkResult;
            ImGui_ImplVulkan_Init(&initInfo, m_api.m_renderPass.getRenderPass());

            {
                VkCommandBuffer commandBuffer = CommandBufferUtil::beginSingleTimeCommands(m_api.m_device.getDevice(),
                                                                                           m_api.m_commandPool);

                {
                    ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
                }
                VkFence fence = CommandBufferUtil::endSingleTimeCommands(m_api.m_device.getDevice(),
                                                                         m_api.m_device.getGraphicsQueue(),
                                                                         commandBuffer);
                vkWaitForFences(m_api.m_device.getDevice(), 1, &fence, VK_TRUE, UINT64_MAX);
                vkDestroyFence(m_api.m_device.getDevice(), fence, nullptr);
                vkFreeCommandBuffers(m_api.m_device.getDevice(), m_api.m_commandPool, 1, &commandBuffer);

                ImGui_ImplVulkan_DestroyFontUploadObjects();
            }
        }
    }

    void VkImguiSystem::shutdown() {
        ImGui_ImplVulkan_Shutdown();
        // ImGui_ImplGlfw_Shutdown();
        // ImGui::DestroyContext();

        vkDestroyDescriptorPool(m_api.m_device.getDevice(), m_pool, nullptr);
    }

    void VkImguiSystem::begin() {
        ImGui_ImplVulkan_NewFrame();
        // ImGui_ImplGlfw_NewFrame();
        // ImGui::NewFrame();
    }

    void VkImguiSystem::end() {
        // ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_api.m_frameObjects[m_api.frameNumber].commandBuffer);

        /* ImGuiIO& io = ImGui::GetIO();
        if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }*/
    }
    
    void VkImguiSystem::drawFramebuffer(const char* name, Ref<Framebuffer> framebuffer) {
        Ref<VkFramebuffer> vkFramebuffer = std::static_pointer_cast<VkFramebuffer>(framebuffer);
        if(VkFramebuffer::sampler == VK_NULL_HANDLE) {
            VkFramebuffer::createGlobalSampler(m_api.m_device);
        }
        u32 imageIndex = vkFramebuffer->m_api.m_frameObjects[m_api.getFrameInFlight()].frameData.imageIndex;
        if(vkFramebuffer->m_images[imageIndex].descriptorSet == VK_NULL_HANDLE){
            vkFramebuffer->m_images[imageIndex].descriptorSet = ImGui_ImplVulkan_AddTexture(VkFramebuffer::sampler, vkFramebuffer->m_images[imageIndex].imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        }

        ImGui::Begin(name);
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        ImGui::Image(vkFramebuffer->m_images[imageIndex].descriptorSet, viewportPanelSize);
        ImGui::End();
        
    }
}