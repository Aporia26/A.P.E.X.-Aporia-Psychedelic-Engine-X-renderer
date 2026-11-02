#pragma once
#include<GLFW/glfw3.h>
#include<iostream>

namespace Apex {
    
    class EditorWindow{
     
     public:
        EditorWindow();
        ~EditorWindow();

       //High level wrapper for opengl

       //getters for window dimension added for viewport of editor
       int GetWindowWidth() const { return m_width; }
       int GetWindowHeight() const { return m_height; }

       //Window Creation
       bool CreateWindow(int width, int height, const std::string& name);
       void Update();
       void Shutdown();
       bool ShouldClose() const;

       //Input Handling
       bool iskeyPressed(int keycode) const;
       bool isMouseButtonPressed(int button) const;
       void GetMousePosition(double& xpos, double& ypos) const;

       GLFWwindow* GetWindow() const { return m_Window; }
     
     private:
       //resize
       static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);

       std::string m_name;
       int m_width = 0;
       int m_height = 0;

       
       GLFWwindow* m_Window = nullptr;

    };
}