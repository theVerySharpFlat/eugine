#include "OpenGLAPI.h"
#include "error.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "eugine/core/application.h"

#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

namespace eg::rendering::GLWrapper {
    OpenGLAPI::OpenGLAPI(Window& window) : m_window(window){
        glfwMakeContextCurrent((GLFWwindow*)window.getNativeWindow());

        int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
        EG_CORE_ASSERT(status, "Failed to initialize Glad");

        int OGL_MINOR_VERSION;
        int OGL_MAJOR_VERSION;
        GLCall(glGetIntegerv(GL_MAJOR_VERSION, &OGL_MAJOR_VERSION));
        GLCall(glGetIntegerv(GL_MINOR_VERSION, &OGL_MINOR_VERSION));

        info("up and running with OpenGL {}.{}", OGL_MAJOR_VERSION, OGL_MINOR_VERSION);
        GLCall(info("RENDERER: {}", glGetString(GL_RENDERER)));
        GLCall(info("VENDOR: {}", glGetString(GL_VENDOR)));
        

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    }

    void OpenGLAPI::swapBuffers() {
        glfwSwapBuffers((GLFWwindow*)m_window.getNativeWindow());
    }

    void OpenGLAPI::setClearColor(glm::vec3 color) {
       GLCall(glClearColor(color.x, color.y, color.z, 1.0));
    }

    void OpenGLAPI::clear() {
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }

    i32 OpenGLAPI::getMaxTexturesPerShader() const {
        i32 count;
        GLCall(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &count));
        return count;
    }

    void OpenGLAPI::imguiInit() {
        // Setup Platform/Renderer backends
        Application& application = Application::get();
        GLFWwindow* window = static_cast<GLFWwindow*>(application.getWindow().getNativeWindow());
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void OpenGLAPI::imguiShutdown() {
        ImGui_ImplOpenGL3_Shutdown();
    }

    void OpenGLAPI::imguiBegin() {
        ImGui_ImplOpenGL3_NewFrame();
    }

    void OpenGLAPI::imguiEnd() {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}