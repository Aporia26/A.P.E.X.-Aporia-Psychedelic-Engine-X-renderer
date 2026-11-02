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
        glClearColor(0.04f, 0.02f, 0.08f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 3. SEPARATION ROUTING MATRIX
        if (m_StateManager.IsInEditorMode()) {
            // MODE A: LEVEL EDITOR WORKING SUITE
            // The editor panels render normally
            m_EditorUI.Render(); 
        } 
        else if (m_StateManager.IsInGameActiveMode()) {
            // MODE B: ACTIVE FPS SANDBOX GAMEPLAY
            // Skip rendering editor panels entirely!
            // Run FPS movement input calculation updates here!
        } 
        else if (m_StateManager.IsInGamePausedMode()) {
            // MODE C: GAME PLAY PAUSED
            // trigger custom Pause Menu ImGui window overlays right here!
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

} 
