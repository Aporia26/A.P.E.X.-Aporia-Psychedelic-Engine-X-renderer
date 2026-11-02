#pragma once
#include "Editor/Window.hpp"
#include "Editor_UI/UI.hpp"
#include "Editor/EngineState.hpp"

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


        bool m_isRunning = false;        
    };
}