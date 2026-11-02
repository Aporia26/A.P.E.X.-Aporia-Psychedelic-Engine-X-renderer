#include "UI.hpp"
#include <imgui.h>
#include <imgui_internal.h>    //for dividing the dockspace layout
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>


namespace Apex {

    EditorUI::EditorUI() {}
    EditorUI::~EditorUI() { Shutdown(); }

    void EditorUI::Initialize(GLFWwindow* window) {
        m_WindowHandle = window;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ApplyPsychedelicTheme();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 450");
    }

    void EditorUI::Render(ViewportCamera& camera, uint32_t sceneTextureID, GLFWwindow* window) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Create a full-screen baseline master background window
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | 
                                       ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | 
                                       ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | 
                                       ImGuiWindowFlags_NoNavFocus;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("APEX_MASTER_WORKSPACE", nullptr, window_flags);
        ImGui::PopStyleVar();

        ImGuiID dockspace_id = ImGui::GetID("ApexEngineMasterDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

        // 2. Automated Dock Layout Splitting (Only runs on the very first boot)
        if (m_FirstFrame) {
            m_FirstFrame = false;

            ImGui::DockBuilderRemoveNode(dockspace_id); // Clear standard workspace
            ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->WorkSize);

            ImGuiID dock_main_id = dockspace_id;
            ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, nullptr, &dock_main_id);
            ImGuiID dock_bottom_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.25f, nullptr, &dock_main_id);
            ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.25f, nullptr, &dock_main_id);

            // Assign panels to their explicit layout docking anchors
            ImGui::DockBuilderDockWindow("Scene Hierarchy", dock_left_id);
            ImGui::DockBuilderDockWindow("Asset Shelf", dock_bottom_id);
            ImGui::DockBuilderDockWindow("Inspector & Toolset", dock_right_id);
            ImGui::DockBuilderDockWindow("Level Viewport", dock_main_id);
            
            ImGui::DockBuilderFinish(dockspace_id);
        }
        ImGui::End(); // End Master Workspace

        // 3. Render Subsystem Sub-Panels
        RenderTopMenuBar();
        RenderLeftHierarchy();
        RenderBottomAssetShelf();
        RenderRightInspector();
        //RenderCenterViewport();
        ViewportPanel::Render(camera, sceneTextureID, window); //our new viewport

        // 4. Final Frame Assembly Rendering Calculations
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void EditorUI::RenderTopMenuBar() {
        if (ImGui::BeginMainMenuBar()) {
            // Left Side: Options
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Load Map Level (.json)")) { /* JSON logic link later */ }
                if (ImGui::MenuItem("Save Map Level (.json)")) { /* JSON logic link later */ }
                ImGui::Separator();
                if (ImGui::MenuItem("Exit Application")) { glfwSetWindowShouldClose(m_WindowHandle, true); }
                ImGui::EndMenu();
            }

            // Center Text: Logo & Project Context Placement
            float windowWidth = ImGui::GetWindowSize().x;
            std::string centerText = "APORIA Engine // Sandbox Environment";
            float textWidth = ImGui::CalcTextSize(centerText.c_str()).x;
            
            ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
            ImGui::TextColored(ImVec4(0.95f, 0.0f, 0.55f, 1.0f), centerText.c_str()); // Highly saturated neon fuchsia text

            ImGui::EndMainMenuBar();
        }
    }

    void EditorUI::RenderLeftHierarchy() {
        ImGui::Begin("Scene Hierarchy");
        ImGui::TextColored(ImVec4(0.0f, 0.9f, 0.9f, 1.0f), "Level Object Nodes:");
        ImGui::Separator();

        // Placeholder array loop to show active scene node objects tree
        if (ImGui::TreeNode("Map Level Core Tree")) {
            ImGui::Selectable("FPS Player Start Point");
            ImGui::Selectable("Neon Signboard Mesh 01");
            ImGui::Selectable("Saturated Ambient Spotlight");
            ImGui::TreePop();
        }
        ImGui::End();
    }

    void EditorUI::RenderBottomAssetShelf() {
        ImGui::Begin("Asset Shelf");
        ImGui::TextColored(ImVec4(0.0f, 0.9f, 0.9f, 1.0f), "Project File Explorer Hierarchy (/assets):");
        ImGui::Separator();

        ImGui::Columns(4, "AssetDirectories", false);
        if (ImGui::Button("[📂] models/")) { /* Open sub-tree folder visualizer */ } ImGui::NextColumn();
        if (ImGui::Button("[📂] audio/"))  { /* Open sub-tree folder visualizer */ } ImGui::NextColumn();
        if (ImGui::Button("[📂] shaders/")) { /* Open sub-tree folder visualizer */ } ImGui::NextColumn();
        if (ImGui::Button("[📄] level1.json")) { /* Open scene definitions */ }
        ImGui::Columns(1);
        ImGui::End();
    }

    void EditorUI::RenderRightInspector() {
        ImGui::Begin("Inspector & Toolset");
        
        // Section A: Spawning Default Basic Meshes & Utilities
        ImGui::TextColored(ImVec4(0.0f, 0.9f, 0.9f, 1.0f), "Spawn Geometry Primitives:");
        if (ImGui::Button("Cube"))   { /* Primitive construction hooks */ } ImGui::SameLine();
        if (ImGui::Button("Sphere")) { /* Primitive construction hooks */ } ImGui::SameLine();
        if (ImGui::Button("Light Source"))  { /* Primitive construction hooks */ }
        
        ImGui::Separator();
        ImGui::Spacing();

        // Section B: Positional Component Transformers
        ImGui::TextColored(ImVec4(0.0f, 0.9f, 0.9f, 1.0f), "Selected Object Transformations:");
        static float position[3] = { 0.0f, 0.0f, 0.0f };
        static float rotation[3] = { 0.0f, 0.0f, 0.0f };
        static float scale[3] = { 1.0f, 1.0f, 1.0f };
        ImGui::DragFloat3("Position", position, 0.05f);
        ImGui::DragFloat3("Rotation", rotation, 0.5f);
        ImGui::DragFloat3("Scale", scale, 0.05f);

        ImGui::Separator();
        ImGui::Spacing();

        // Section C: Specialized Component Modifiers Addition Tools
        ImGui::TextColored(ImVec4(0.0f, 0.9f, 0.9f, 1.0f), "Add Engine Components:");
        if (ImGui::Button("Add Rigid Body Physics (PhysX)")) { /* Physics framework injection target */ }
        if (ImGui::Button("Add Spatial 3D Audio Node (OpenAL)")) { /* Sound engine injection target */ }

        ImGui::End();
    }

    
    /*
    // ###########################################################
    // Replaced This Placeholder By the new actual Viewport...
    // ###########################################################

    void EditorUI::RenderCenterViewport() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Level Viewport");
        
        ImGui::Text("[3D Viewport Window Frame - Level Preview Grid Space]");
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), 
            "Hold Right-Click + WASD to fly camera. Render target texture binds here via glTexture IDs.");

        ImGui::End();
        ImGui::PopStyleVar();
    }
*/
    void EditorUI::ApplyPsychedelicTheme() {
        // We configure a high-contrast, deeply moody palette matching Wong Kar-wai color styles
       
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        style.WindowRounding = 0.0f;
        style.FrameRounding = 2.0f;

        colors[ImGuiCol_WindowBg]             = ImVec4(0.04f, 0.02f, 0.08f, 1.0f);  // Deep twilight indigo night canvas
        colors[ImGuiCol_Header]               = ImVec4(0.35f, 0.00f, 0.50f, 1.0f);  // Heavy dark violet tabs
        colors[ImGuiCol_HeaderHovered]        = ImVec4(0.70f, 0.00f, 0.50f, 1.0f);  // Vibrating purple highlights
        colors[ImGuiCol_HeaderActive]         = ImVec4(0.95f, 0.00f, 0.55f, 1.0f);  // High saturation neon fuchsia choice
        colors[ImGuiCol_Button]               = ImVec4(0.12f, 0.05f, 0.22f, 1.0f);  // Darker violet actions panel button
        colors[ImGuiCol_ButtonHovered]        = ImVec4(0.70f, 0.00f, 0.50f, 1.0f);  
        colors[ImGuiCol_ButtonActive]         = ImVec4(0.95f, 0.00f, 0.55f, 1.0f);  
        colors[ImGuiCol_TitleBgActive]        = ImVec4(0.16f, 0.04f, 0.28f, 1.0f);  // Focused frame title strip
        colors[ImGuiCol_Tab]                  = ImVec4(0.08f, 0.03f, 0.16f, 1.0f);
        colors[ImGuiCol_TabHovered]           = ImVec4(0.70f, 0.00f, 0.50f, 1.0f);
        colors[ImGuiCol_ButtonActive]           = ImVec4(0.35f, 0.00f, 0.50f, 1.0f);
        colors[ImGuiCol_FrameBg]              = ImVec4(0.08f, 0.03f, 0.16f, 1.0f); // Dark fields inputs backgrounds 
        colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.24f, 0.08f, 0.38f, 1.0f); 
        colors[ImGuiCol_FrameBgActive]        = ImVec4(0.35f, 0.00f, 0.50f, 1.0f); 
    } 

    void EditorUI::Shutdown() 
    { 
        // Ensure the ImGui main core context exists before invoking driver sweeps
        if (ImGui::GetCurrentContext() != nullptr) {
            
            // Check if the renderer backend data is still active before clearing
            if (ImGui::GetIO().BackendRendererUserData != nullptr) {
                ImGui_ImplOpenGL3_Shutdown(); 
            }
            
            // Check if the platform window backend context is still active before clearing
            if (ImGui::GetIO().BackendPlatformUserData != nullptr) {
                ImGui_ImplGlfw_Shutdown(); 
            }
            
            ImGui::DestroyContext(); 
        }
    }
}
