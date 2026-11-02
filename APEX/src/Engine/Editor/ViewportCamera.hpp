#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<algorithm>

namespace Apex{

    class ViewportCamera
    {
    public:
        ViewportCamera(float fov = 45.0f, float aspect = 1.778f, float zNear = 0.1f, float zFar = 1000.0f)
        : m_FOV(fov), m_AspectRatio(aspect), m_NearClip(zNear), m_FarClip(zFar) { UpdateVectors(); }

         // WASD keyboard updates when flying in viewport
        void ProcessKeyboard(int direction, float deltaTime, bool speedBoost) {
        float velocity = m_MoveSpeed * deltaTime;

            if (speedBoost) velocity *= 2.5f;                       // Hold shift to sprint through scene
            if (direction == 0) m_Position += m_Forward * velocity; // W
            if (direction == 1) m_Position -= m_Forward * velocity; // S
            if (direction == 2) m_Position -= m_Right * velocity;   // A
            if (direction == 3) m_Position += m_Right * velocity;   // D
            if (direction == 4) m_Position += m_WorldUp * velocity; // E (Fly Up)
            if (direction == 5) m_Position -= m_WorldUp * velocity; // Q (Fly Down)
        }

         // Handles mouse rotation when holding Right-Mouse-Button
        void ProcessMouseMovement(float xoffset, float yoffset) {
            xoffset *= m_Sensitivity;
            yoffset *= m_Sensitivity;

            m_Yaw   += xoffset;
            m_Pitch += yoffset;

            // Prevent screen flipping ...
            m_Pitch = std::clamp(m_Pitch, -89.0f, 89.0f);
            UpdateVectors();
        }

        // Handles mouse scroll zoom 
        void ProcessMouseScroll(float yoffset) {
            m_Position += m_Forward * (yoffset * m_ZoomSpeed);
        }
        void SetAspectRatio(float width, float height) {
            if (width <= 0.0f || height <= 0.0f) return;
            m_AspectRatio = width / height;
        }

        glm::mat4 GetViewMatrix() const { return glm::lookAt(m_Position, m_Position + m_Forward, m_Up); }
        glm::mat4 GetProjectionMatrix() const { return glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip); }

    private:
        void UpdateVectors() 
        {
            glm::vec3 front;
            front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
            front.y = sin(glm::radians(m_Pitch));
            front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
            
            m_Forward = glm::normalize(front);
            m_Right   = glm::normalize(glm::cross(m_Forward, m_WorldUp));
            m_Up      = glm::normalize(glm::cross(m_Right, m_Forward));
        }
    
     private:
        glm::vec3 m_Position{ 0.0f, 6.0f, 15.0f };
        glm::vec3 m_Forward{ 0.0f, 0.0f, -1.0f };
        glm::vec3 m_Up{ 0.0f, 1.0f, 0.0f };
        glm::vec3 m_Right{ 1.0f, 0.0f, 0.0f };
        glm::vec3 m_WorldUp{ 0.0f, 1.0f, 0.0f };

        float m_Yaw{ -90.0f };
        float m_Pitch{ -25.0f };
        float m_MoveSpeed{ 10.0f };
        float m_Sensitivity{ 0.12f };
        float m_ZoomSpeed{ 2.0f };

        float m_FOV, m_AspectRatio, m_NearClip, m_FarClip;

    };
}