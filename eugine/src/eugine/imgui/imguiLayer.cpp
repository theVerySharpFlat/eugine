//
// Created by aiden on 9/3/21.
//

#include <eugine/core/application.h>
#include "imguiLayer.h"

#include "imgui/backends/imgui_impl_glfw.h"


#include "GLFW/glfw3.h"
#include <cstring>

enum GlfwClientApi
        {
    GlfwClientApi_Unknown,
    GlfwClientApi_OpenGL,
    GlfwClientApi_Vulkan
        };

struct ImGui_Data {
    GLFWwindow*             Window;
    GlfwClientApi           ClientApi = GlfwClientApi_OpenGL;
    double                  Time;
    GLFWwindow*             MouseWindow;
    bool                    MouseJustPressed[ImGuiMouseButton_COUNT];
    GLFWcursor*             MouseCursors[ImGuiMouseCursor_COUNT];
    bool                    InstalledCallbacks;

    // Chain GLFW callbacks: our callbacks will call the user's previously installed callbacks, if any.
    GLFWwindowfocusfun      PrevUserCallbackWindowFocus;
    GLFWcursorenterfun      PrevUserCallbackCursorEnter;
    GLFWmousebuttonfun      PrevUserCallbackMousebutton;
    GLFWscrollfun           PrevUserCallbackScroll;
    GLFWkeyfun              PrevUserCallbackKey;
    GLFWcharfun             PrevUserCallbackChar;
    GLFWmonitorfun          PrevUserCallbackMonitor;

    ImGui_Data()   { memset(this, 0, sizeof(*this)); }
};

static ImGui_Data* ImGui_GetBackendData() {
    return ImGui::GetCurrentContext() ? (ImGui_Data*)ImGui::GetIO().BackendPlatformUserData : nullptr;
}

namespace eg {

    ImGuiLayer::ImGuiLayer() : Layer("ImGui Layer") {

    }

    ImGuiLayer::~ImGuiLayer() {

    }

    void ImGuiLayer::onAttach() {
        eg::trace("setting up imgui layer");
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
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
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
    }

    void ImGuiLayer::onDetach() {
        // ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::begin() {
        // ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::end() {
        //do some setup
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::get();
        io.DisplaySize = ImVec2(app.getWindow().getWidth(), app.getWindow().getHeight());

        //do some rendering
        ImGui::Render();
        //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    }

    void ImGuiLayer::updateWindows() {
        ImGuiIO& io = ImGui::GetIO();
        if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }


    void ImGuiLayer::onImGuiRender() {
        static bool show = true;
        ImGui::ShowDemoWindow(&show);
    }


}
