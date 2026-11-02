#pragma once
#include <GLFW/glfw3.h>
#include "ViewportPanel.hpp"

namespace Apex {

    class EditorUI
    {
    public:
        EditorUI();
        ~EditorUI();

        void Initialize(GLFWwindow* window);
        void Render(ViewportCamera& camera, uint32_t sceneTextureID, GLFWwindow* window);
        void Shutdown();

    private:
        void RenderTopMenuBar();
        void RenderLeftHierarchy();
        void RenderBottomAssetShelf();
        void RenderRightInspector();
        void RenderCenterViewport();

        //my default theme
        void ApplyPsychedelicTheme();

        GLFWwindow* m_WindowHandle = nullptr;
        bool m_FirstFrame = true; //track initial docking setup 
    };
}