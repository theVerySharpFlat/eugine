//
// Created by aiden on 9/3/21.
//

#include <eugine/core/application.h>
#include "imguiLayer.h"
#include "eugine/platform/OpenGL/imguiOpenGLRenderer.h"
#include "GLFW/glfw3.h"
#include <glad/glad.h>

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
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;


        //temp
        io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
        io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
        io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
        io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
        io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
        io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
        io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
        io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
        io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

        ImGui_ImplOpenGL3_Init("#version 410");
    }

    ImGuiLayer::~ImGuiLayer() {

    }

    void ImGuiLayer::onAttach() {
        Layer::onAttach();
    }

    void ImGuiLayer::onDetach() {
        Layer::onDetach();
    }

    void ImGuiLayer::onUpdate() {


        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::get();
        //EG_CORE_TRACE("window size: ({0}, {1})", app.getWindow().getWidth(), app.getWindow().getHeight());
        //std::cout << app.getWindow().getWidth() << " " << app.getWindow().getHeight() << std::endl;
        io.DisplaySize = ImVec2((float)app.getWindow().getWidth(), (float)app.getWindow().getHeight());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        float time = (float)glfwGetTime();
        io.DeltaTime = m_time > 0.0f ? (time - m_time) : (1.0f / 60.0f);
        m_time = time;

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiLayer::onEvent(Event &e) {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<MouseButtonPressedEvent>(EG_BIND_EVENT_FN(ImGuiLayer::mouseButtonPressedCallback));
        dispatcher.dispatch<MouseButtonReleasedEvent>(EG_BIND_EVENT_FN(ImGuiLayer::mouseButtonReleasedCallback));
        dispatcher.dispatch<KeyPressedEvent>(EG_BIND_EVENT_FN(ImGuiLayer::keyPressedCallback));
        dispatcher.dispatch<KeyReleasedEvent>(EG_BIND_EVENT_FN(ImGuiLayer::keyReleasedCallback));
        dispatcher.dispatch<MouseMovedEvent>(EG_BIND_EVENT_FN(ImGuiLayer::mouseMovedCallback));
        dispatcher.dispatch<WindowResizeEvent>(EG_BIND_EVENT_FN(ImGuiLayer::windowResizeCallback));
        dispatcher.dispatch<KeyTypedEvent>(EG_BIND_EVENT_FN(ImGuiLayer::keyTypedEvent));
        

    }


    bool ImGuiLayer::scrollCallback(MouseScrolledEvent &e) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheel += e.getXOffset();
        io.MouseWheelH += e.getYOffset();

        return false;
    }

    bool ImGuiLayer::keyPressedCallback(KeyPressedEvent &e) {
        ImGuiIO& io = ImGui::GetIO();

        io.KeysDown[e.getKeyCode()] = true;

        io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

        return false;
    }

    bool ImGuiLayer::keyReleasedCallback(KeyReleasedEvent &e) {

        ImGuiIO& io = ImGui::GetIO();

        io.KeysDown[e.getKeyCode()] = false;

        return false;
    }

    bool ImGuiLayer::mouseMovedCallback(MouseMovedEvent &e) {
        ImGuiIO& io = ImGui::GetIO();

        io.MousePos = ImVec2(e.getX(), e.getY());

        return false;
    }

    bool ImGuiLayer::windowResizeCallback(WindowResizeEvent &e) {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float)e.getWidth(), (float)e.getWidth());
        io.DisplayFramebufferScale = ImVec2(1.0f,1.0f);
        glViewport(0,0,(GLint)e.getWidth(), (GLint)e.getHeight());

        return false;
    }

    bool ImGuiLayer::mouseButtonPressedCallback(MouseButtonPressedEvent &e) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.getMouseButton()] = true;

        return false;
    }

    bool ImGuiLayer::mouseButtonReleasedCallback(MouseButtonReleasedEvent &e) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.getMouseButton()] = false;

        return false;
    }

    bool ImGuiLayer::keyTypedEvent(KeyTypedEvent & e) {
        ImGuiIO& io = ImGui::GetIO();
        int keycode = e.getKeyCode();

        if(keycode > 0 && keycode < 0x10000)
            io.AddInputCharacter((unsigned short)keycode);

        return false;
    }
}