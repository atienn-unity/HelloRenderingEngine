////////////////////////////////////////////////////////////////////
//                Copyright Oliver J. Rosten 2024.                //
// Distributed under the GNU GENERAL PUBLIC LICENSE, Version 3.0. //
//    (See accompanying file LICENSE.md or copy at                //
//          https://www.gnu.org/licenses/gpl-3.0.en.html)         //
////////////////////////////////////////////////////////////////////

#include "GLFW/glfw3.h"
#include <iostream>


void errorCallback(int error, const char* description) {
    std::cerr << std::format("Error - {}: {}\n", error, description);
}


class Window {
    GLFWwindow* m_Window{};

    inline static void set_window_creation_hints_preset() {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    }

public:
    Window(int width, int height, const char* title, bool setPresetCreationHints = false) {
        // Don't set the global window hints by default as to minimize accidental side-effects.
        if (setPresetCreationHints) set_window_creation_hints_preset();

        m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!m_Window)
            throw std::runtime_error("Failed to create GLFW window");
    }

    ~Window() {
        glfwDestroyWindow(m_Window);
    }

    // As the constructor and destructor are mostly pure (from my understanding), we can preserve the copy and assignment operators.
    // This way, we'll still be able to create multiple windows if we want to.

    GLFWwindow& get() {
        if (!m_Window) throw std::runtime_error("Failed to retrieve window!");
        return *m_Window;
    }
};


// Somewhat forced to make this into its own scope/function so that appWindow gets 
// destroyed before glfwTerminate() is called (we get an error otherwise). Not ideal.
void main_loop() {
    Window appWindow(800, 600, "Hello GLFW", true);
    // this call is intentionally left outside the Window constructor as it has side-effects (detaches any current context)
    glfwMakeContextCurrent(&appWindow.get());

    while (!glfwWindowShouldClose(&appWindow.get())) {
        // Render here (optional)
        //glClear(GL_COLOR_BUFFER_BIT);

        // Swap front and back buffers
        glfwSwapBuffers(&appWindow.get());

        // Poll for and process events
        glfwPollEvents();
    }
}

int main() {
    try {
        glfwSetErrorCallback(errorCallback);
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW\n");
            return -1;
        }

        main_loop();

        glfwTerminate();
        return 0;
    }

    catch (const std::exception& e)
    {
        std::cerr << e.what();
        glfwTerminate();
    }
    catch (...)
    {
        std::cerr << "Unrecognized error\n";
        glfwTerminate();
    }
}
