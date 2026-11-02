#include "Engine.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace Apex {
    
    Engine::Engine() 
     : m_isRunning(false) {}

    Engine::~Engine() { Shutdown(); }

    bool Engine::Init()
    {
         std::cout << "APEX ENGINE [System Status]: Booting core subsystems...\n";

        // Initialize the low-level window & OpenGL context loader (GLAD) and make OS
        if (!m_WindowModule.CreateWindow(1280, 720, "APEX Engine - Workspace")) {
            std::cerr << "CRITICAL ERROR: Window framework failed to build context.\n";
            return false;
        }

        // Initialize the developer ImGui UI layout framework
        m_EditorUI.Initialize(m_WindowModule.GetWindow());
        
        //Viewport Init
        InitViewportFramebuffer();
        CompileBasicGridShader();

        m_isRunning = true;
        std::cout << "APEX ENGINE [System Status]: All systems online.\n";
        return true;
    }

    void Engine::Run()
    {
        GLFWwindow* nativeWin = m_WindowModule.GetWindow();

        while(m_isRunning && !m_WindowModule.ShouldClose())
        {

        // 1. Process Core Keyboard State Overrides
        m_StateManager.Update(nativeWin);

        // 2. Clear buffers 
       // glClearColor(0.04f, 0.02f, 0.08f, 1.0f); 
       // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 3. SEPARATION ROUTING MATRIX
        if (m_StateManager.IsInEditorMode()) {
            // MODE A: LEVEL EDITOR WORKING SUITE
            // The editor panels render normally

            //1. our 3d level general
             glBindFramebuffer(GL_FRAMEBUFFER, m_ViewportFBO);
             glEnable(GL_DEPTH_TEST);

            // Force viewport to match steady 1080p raw texture sheet asset scale
             glViewport(0, 0, 1920, 1080); 
             glClearColor(0.08f, 0.06f, 0.12f, 1.0f); // Twilight space canvas hue
             glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

             // Compute perspective transformation matrices tracking camera spatial state
             glm::mat4 vpMatrix = m_EditorCamera.GetProjectionMatrix() * m_EditorCamera.GetViewMatrix();
                
             // Render 3D Ground Orientation Grid directly onto the active FBO texture canvas sheet
             ViewportPanel::DrawOriginGrid(vpMatrix, m_GridShader, m_GridVAO);

             //2. render imgui
             glBindFramebuffer(GL_FRAMEBUFFER, 0); // Restore standard OS window buffer path channel
                
             int scrWidth, scrHeight;
             glfwGetFramebufferSize(nativeWin, &scrWidth, &scrHeight);
             glViewport(0, 0, scrWidth, scrHeight); // Sync display coordinates locally
                
             glClearColor(0.04f, 0.02f, 0.08f, 1.0f); // Default deep psychedelic background
             glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

             //Multi Dock Editor Layout
            m_EditorUI.Render(m_EditorCamera, m_ViewportTexture, nativeWin); 

            // Project the rendered 3D level engine context directly inside "Level Viewport" ImGui window frame
             //ViewportPanel::Render(m_EditorCamera, m_ViewportTexture, nativeWin);
        } 
        else if (m_StateManager.IsInGameActiveMode()) {
            // MODE B: ACTIVE FPS SANDBOX GAMEPLAY
            // Skip rendering editor panels entirely!
            // Run FPS movement input calculation updates here!

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, m_WindowModule.GetWindowWidth(), m_WindowModule.GetWindowHeight());
            glClearColor(0.02f, 0.01f, 0.04f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        } 
        else if (m_StateManager.IsInGamePausedMode()) {
            // MODE C: GAME PLAY PAUSED
            // trigger custom Pause Menu ImGui window overlays right here!

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClearColor(0.02f, 0.01f, 0.04f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
        }
            //Hardware sync
            m_WindowModule.Update();
        }
    }

    void Engine::Shutdown() {
        if (!m_isRunning) return;

        std::cout << "APEX ENGINE [System Status]: Executing teardown sequence...\n";

        // 1. Terminate ImGui memory contexts cleanly
        m_EditorUI.Shutdown();

        // 2. Safely release GLFW hooks and destroy rendering contexts
        m_WindowModule.Shutdown();

        m_isRunning = false;
        std::cout << "APEX ENGINE [System Status]: Shutdown complete. Safe to exit.\n";
    }

    // VIEWPORT FRAMEBUFFER INITIALIZATION
    void Engine::InitViewportFramebuffer() {
        glGenFramebuffers(1, &m_ViewportFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_ViewportFBO);

        glGenTextures(1, &m_ViewportTexture);
        glBindTexture(GL_TEXTURE_2D, m_ViewportTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ViewportTexture, 0);

        glGenRenderbuffers(1, &m_ViewportRBO);
        glBindRenderbuffer(GL_RENDERBUFFER, m_ViewportRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1920, 1080); 
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_ViewportRBO);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    
    // DYNAMIC VIEWPORT GRID SHADER INTERFACING ASSEMBLY
    void Engine::CompileBasicGridShader() {
        const char* vertexShaderSource = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec3 aColor;
            out vec3 vColor;
            uniform mat4 u_ViewProjection;
            void main() {
                vColor = aColor;
                gl_Position = u_ViewProjection * vec4(aPos, 1.0);
            }
        )";
        const char* fragmentShaderSource = R"(
            #version 330 core
            in vec3 vColor;
            out vec4 FragColor;
            void main() {
                FragColor = vec4(vColor, 1.0);
            }
        )";

        uint32_t vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertexShaderSource, NULL);
        glCompileShader(vs);

        uint32_t fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragmentShaderSource, NULL);
        glCompileShader(fs);

        m_GridShader = glCreateProgram();
        glAttachShader(m_GridShader, vs);
        glAttachShader(m_GridShader, fs);
        glLinkProgram(m_GridShader);

        glDeleteShader(vs);
        glDeleteShader(fs);
    }
} 
