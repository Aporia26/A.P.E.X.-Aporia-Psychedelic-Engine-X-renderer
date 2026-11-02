#pragma once
#include "Editor/Window.hpp"
#include "Editor_UI/UI.hpp"
#include "Editor/EngineState.hpp"
#include "Editor_UI/ViewportPanel.hpp"
#include <cstdint> // Required for clean uint32_t

namespace Apex {

    class Engine
    {
    public:
        Engine();
        ~Engine();

        //high level lifecycle
        bool Init();
        void Run();
        void Shutdown();

    private:
       //binding hardware modules to engine core
        EditorWindow m_WindowModule;
        EditorUI m_EditorUI;
        EngineState m_StateManager;

        ViewportCamera m_EditorCamera;
    
    // Low level OpenGL rendering IDs for the framebuffer viewport render
    uint32_t m_ViewportFBO = 0;
    uint32_t m_ViewportTexture = 0;
    uint32_t m_ViewportRBO = 0;
    //grid graphic
    uint32_t m_GridShader = 0;
    uint32_t m_GridVAO = 0;

    void InitViewportFramebuffer();
    void CompileBasicGridShader();


        bool m_isRunning = false;        
    };
}