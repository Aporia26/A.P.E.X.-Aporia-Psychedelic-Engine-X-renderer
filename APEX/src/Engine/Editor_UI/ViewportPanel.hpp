#pragma once
#include<imgui.h>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<vector>
#include "../Editor/ViewportCamera.hpp"

namespace Apex{

    class ViewportPanel
    {
    public:
        static void Render(ViewportCamera& camera, uint32_t sceneTextureID, GLFWwindow* window) {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Level Viewport");

            bool isHovered = ImGui::IsWindowHovered();
            ImVec2 panelSize = ImGui::GetContentRegionAvail();

            // 1. Dynamic projection correction
            camera.SetAspectRatio(panelSize.x, panelSize.y);

            // 2. Input routing loop (Processes movement only when holding Right Click over viewport)
            ImGuiIO& io = ImGui::GetIO();
            if (isHovered) {
                if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
                    // Lock cursor locally when adjusting view angle
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                    camera.ProcessMouseMovement(io.MouseDelta.x, -io.MouseDelta.y);

                    // Forward WASD flight coordinates
                    float dt = io.DeltaTime;
                    bool shift = ImGui::IsKeyDown(ImGuiKey_LeftShift);
                    if (ImGui::IsKeyDown(ImGuiKey_W)) camera.ProcessKeyboard(0, dt, shift);
                    if (ImGui::IsKeyDown(ImGuiKey_S)) camera.ProcessKeyboard(1, dt, shift);
                    if (ImGui::IsKeyDown(ImGuiKey_A)) camera.ProcessKeyboard(2, dt, shift);
                    if (ImGui::IsKeyDown(ImGuiKey_D)) camera.ProcessKeyboard(3, dt, shift);
                    if (ImGui::IsKeyDown(ImGuiKey_E)) camera.ProcessKeyboard(4, dt, shift);
                    if (ImGui::IsKeyDown(ImGuiKey_Q)) camera.ProcessKeyboard(5, dt, shift);
                } else {
                    // Restore standard click state pointers when released
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                }

                if (io.MouseWheel != 0.0f) {
                    camera.ProcessMouseScroll(io.MouseWheel);
                }
            }

            // 3. Render Level Preview Target. Invert Y alignment vectors (0,1 to 1,0) to correct OpenGL tex flip
            ImGui::Image((void*)(intptr_t)sceneTextureID, panelSize, ImVec2(0, 1), ImVec2(1, 0));

            ImGui::End();
            ImGui::PopStyleVar();
        }

        // Draw structural 3D lines GRID directly onto the Framebuffer layer
        static void DrawOriginGrid(const glm::mat4& viewProj, uint32_t gridShaderID, uint32_t& gridVAO, int size = 40, int step = 2) {
            static uint32_t gridVBO = 0;
            static int vertexCount = 0;

            // Generate layout arrays only once during runtime execution initialization
            if (gridVAO == 0) {
                std::vector<float> vertices;
                for (int i = -size; i <= size; i += step) {
                    // Lines tracking down the Z axis
                    vertices.insert(vertices.end(), { (float)i, 0.0f, (float)-size,  0.5f, 0.5f, 0.5f });
                    vertices.insert(vertices.end(), { (float)i, 0.0f, (float)size,   0.5f, 0.5f, 0.5f });
                    // Lines tracking down the X axis
                    vertices.insert(vertices.end(), { (float)-size, 0.0f, (float)i,  0.5f, 0.5f, 0.5f });
                    vertices.insert(vertices.end(), { (float)size,  0.0f, (float)i,  0.5f, 0.5f, 0.5f });
                }
                vertexCount = (int)vertices.size() / 6;

                glGenVertexArrays(1, &gridVAO);
                glGenBuffers(1, &gridVBO);
                glBindVertexArray(gridVAO);
                glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
                glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

                // Position layout data
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);
                // Color layer attributes data
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
                glEnableVertexAttribArray(1);
            }

            glUseProgram(gridShaderID);
            glUniformMatrix4fv(glGetUniformLocation(gridShaderID, "u_ViewProjection"), 1, GL_FALSE, &viewProj[0][0]);
            
            glBindVertexArray(gridVAO);
            glDrawArrays(GL_LINES, 0, vertexCount);
            glBindVertexArray(0);
        }
    };
}