#include "EngineState.hpp"
#include <iostream>

namespace Apex {

       EngineState::EngineState() 
        : m_CurrentState(EngineMode::EditorMode) {}

    EngineState::~EngineState() {}

    void EngineState::Update(GLFWwindow* window) {
        // --- 1. HANDLE TAB KEY LOGIC (Editor <-> Game Mode Toggle) ---
        bool tabPressedNow = (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS);
        
        if (tabPressedNow && !m_TabKeyWasPressed) {
            // Key Just Pressed Down (Edge Triggered)
            if (m_CurrentState == EngineMode::EditorMode) {
                m_CurrentState = EngineMode::GameModeActive;
                std::cout << "APEX STATE: Switched to GAME MODE (Simulation Active)\n";
                // Hide mouse cursor and lock it to window center for FPS look mechanics
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            } 
            else {
                // If in active game or paused game, TAB forces an escape back to level editor
                m_CurrentState = EngineMode::EditorMode;
                std::cout << "APEX STATE: Returned to EDITOR MODE\n";
                // Restore standard mouse pointer visibility to click on panels
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }
        m_TabKeyWasPressed = tabPressedNow; // Cache keyboard current frame state

        // --- 2. HANDLE P KEY LOGIC (Game Active <-> Game Paused Toggle) ---
        bool pPressedNow = (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS);

        if (pPressedNow && !m_PKeyWasPressed) {
            // P key only triggers if we are inside the Sandbox Game loop state matrix
            if (m_CurrentState == EngineMode::GameModeActive) {
                m_CurrentState = EngineMode::GameModePaused;
                std::cout << "APEX STATE: Game PAUSED\n";
                // Unlock cursor during pause menu so user can click "Resume" or "Quit"
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            } 
            else if (m_CurrentState == EngineMode::GameModePaused) {
                m_CurrentState = EngineMode::GameModeActive;
                std::cout << "APEX STATE: Game RESUMED\n";
                // Re-lock mouse cursor for FPS look tracking
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }
        m_PKeyWasPressed = pPressedNow;
    }
}