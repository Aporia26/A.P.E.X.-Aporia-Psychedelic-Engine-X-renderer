#include "Window.hpp"
#include <glad/glad.h> //first
#include <GLFW/glfw3.h> //os

namespace Apex {

        EditorWindow::EditorWindow() {}
        EditorWindow::~EditorWindow() { Shutdown(); }
    
        //Window
        bool EditorWindow::CreateWindow(int width, int height, const std::string& name)
        {
             m_width = width;
             m_height = height;
             m_name = name;

             if(!glfwInit())
            {
              std::cerr << "CRITICAL: GLFW failed platform layer init.\n";
              return false;
            }

            //create context
          // STRATEGY 1: Try your desired standard OpenGL 3.3 Core Profile
    std::cout << "APEX WINDOW: Attempting to bind OpenGL 3.3 Core Profile...\n";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required for many modern laptop drivers

    m_Window = glfwCreateWindow(m_width, m_height, m_name.c_str(), nullptr, nullptr);

    // STRATEGY 2: If Core Profile returns NULL, switch to a Safe-Mode Fallback Loop
    if (!m_Window) {
        std::cout << "APEX WINDOW [Warning]: Strict Core Profile rejected by driver.\n";
        std::cout << "APEX WINDOW: Triggering safe compatibility fallback handshake...\n";
        
        // Reset all constraints completely back to absolute basics
        glfwDefaultWindowHints(); 
        
        // Let the driver choose its highest available backwards-compatible window channel
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE); 

        m_Window = glfwCreateWindow(m_width, m_height, m_name.c_str(), nullptr, nullptr);
    }

    // STRATEGY 3: Complete Hardware Failure Safety Net
    if (!m_Window) {
        std::cerr << "CRITICAL ERROR: Native OS window binding failed across all profiles.\n";
        glfwTerminate();
        return false;
    }

            glfwMakeContextCurrent(m_Window);

            glfwSetWindowUserPointer(m_Window, this);
            glfwSetFramebufferSizeCallback(m_Window, FramebufferResizeCallback);

            //glad driver
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
            std::cerr << "CRITICAL: GLAD modern OpenGL function loader crashed.\n";
            return false;
            }

            //The Viewport 
             glViewport(0, 0, m_width, m_height);
             glfwSwapInterval(1); // Lock to VSync

             return true; 
        }

       void EditorWindow::Update()
        {
            glfwPollEvents();
            glfwSwapBuffers(m_Window);
        }

       void EditorWindow::Shutdown()
       {
         if (m_Window) {
            glfwDestroyWindow(m_Window);
            glfwTerminate();
            m_Window = nullptr;
        }
       }

        bool EditorWindow::ShouldClose() const 
        {
        return glfwWindowShouldClose(m_Window) != 0;
        }

        //Input
        bool EditorWindow::iskeyPressed(int keycode) const
        {
            int state = glfwGetKey(m_Window, keycode);
            return state == GLFW_PRESS || state == GLFW_REPEAT;
        }

        bool EditorWindow::isMouseButtonPressed(int button) const
        {
            return glfwGetMouseButton(m_Window, button) == GLFW_PRESS;
        }    

        void EditorWindow::GetMousePosition(double& xpos, double& ypos) const 
        {
        glfwGetCursorPos(m_Window, &xpos, &ypos);
        }

        //resize Window
         void EditorWindow::FramebufferResizeCallback(GLFWwindow* window, int width, int height) 
         {
          auto* instance = static_cast<EditorWindow*>(glfwGetWindowUserPointer(window));
          if (instance) {
            instance->m_width = width;
            instance->m_height = height;
            glViewport(0, 0, width, height); // Dynamically update view space bounds
        }
    }
}