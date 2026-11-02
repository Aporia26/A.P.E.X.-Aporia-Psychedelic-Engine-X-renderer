#pragma once
#include <GLFW/glfw3.h>

namespace Apex {

   enum class EngineMode
   {
    EditorMode,
    GameModeActive,
    GameModePaused
   };

    class EngineState
    {
    public:
        EngineState();
        ~EngineState();

        void Update(GLFWwindow* window);

        //state query
        bool IsInEditorMode() const {  return m_CurrentState == EngineMode::EditorMode; }
        bool IsInGameActiveMode() const  { return m_CurrentState == EngineMode::GameModeActive; }
        bool IsInGamePausedMode() const  { return m_CurrentState == EngineMode::GameModePaused; }

        //UI Buttons method to change mode
         void SetState(EngineMode newState) { m_CurrentState = newState; }
        void ForceReturnToEditor() { m_CurrentState = EngineMode::EditorMode; }


    private:
        EngineMode m_CurrentState = EngineMode::EditorMode;

        //prevent rapid key clicking crashes
         bool m_TabKeyWasPressed = false;
        bool m_PKeyWasPressed = false;

    };

}