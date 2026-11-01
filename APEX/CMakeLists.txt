cmake_minimum_required(VERSION 3.22)
project(APEX_Engine VERSION 1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =========================================================================
# 🚀 MIN-GW LINKER OVERRIDES (THE PERMANENT WINMAIN RESOLUTION)
# =========================================================================
# This absolutely commands the linker to map the application to standard int main()
if(NOT MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-subsystem,console")
    add_compile_definitions(GLFW_INCLUDE_NONE)
else()
    add_compile_options(/MP)
endif()

# Forces the final executable binary directly into your main project workspace folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

# =========================================================================
# 📦 CENTRALIZED ECOSYSTEM LOCATION MAPPING
# =========================================================================
set(GRAPHICS_ROOT "C:/Developer")
set(DEVELOPER_SRC_DIR "${GRAPHICS_ROOT}/src")
set(DEVELOPER_INC_DIR "${GRAPHICS_ROOT}/include")
set(DEVELOPER_LIB_DIR "${GRAPHICS_ROOT}/lib")

# Globally maps include headers for third-party utilities
include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${DEVELOPER_INC_DIR}"
    "${DEVELOPER_INC_DIR}/imgui"
    "${DEVELOPER_INC_DIR}/imgui/backends"
)

link_directories("${DEVELOPER_LIB_DIR}")

# =========================================================================
# 📂 THE EXPLICIT FILE BLUEPRINT (NO DUPLICATION OVERLAPS)
# =========================================================================
# 1. Custom Core Engine and Sandbox Source Files
set(PROJECT_SOURCES
    "src/Sandbox/entry.cpp"
    "src/Engine/Engine.hpp"
    "src/Engine/Engine.cpp"
    "src/Engine/Editor/Window.hpp"
    "src/Engine/Editor/Window.cpp"
    "src/Engine/Editor_UI/UI.hpp"
    "src/Engine/Editor_UI/UI.cpp"
)

# 2. Centralized ImGui Source Files (Docking Branch Layout)
set(IMGUI_SOURCES
    "${DEVELOPER_SRC_DIR}/imgui/imgui.cpp"
    "${DEVELOPER_SRC_DIR}/imgui/imgui_draw.cpp"
    "${DEVELOPER_SRC_DIR}/imgui/imgui_tables.cpp"
    "${DEVELOPER_SRC_DIR}/imgui/imgui_widgets.cpp"
    "${DEVELOPER_SRC_DIR}/imgui/backends/imgui_impl_glfw.cpp"
    "${DEVELOPER_SRC_DIR}/imgui/backends/imgui_impl_opengl3.cpp"
)

# 3. Centralized Glad Loader file
set(GLAD_SOURCE "${DEVELOPER_SRC_DIR}/glad.c")

# =========================================================================
# 🔨 CONSTRUCT UNIFIED STANDALONE EXECUTABLE
# =========================================================================
add_executable(${PROJECT_NAME} 
    ${PROJECT_SOURCES} 
    ${IMGUI_SOURCES}
    ${GLAD_SOURCE}
)

# =========================================================================
# 🔍 TARGET INCLUDE DIRECTORIES
# =========================================================================
target_include_directories(${PROJECT_NAME} PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${DEVELOPER_INC_DIR}"                    
    "${DEVELOPER_INC_DIR}/imgui"              
    "${DEVELOPER_INC_DIR}/imgui/backends"     
)

# =========================================================================
# 🔗 THE LINKER STACK (OpenGL, ImGui, OpenAL, Assimp Alignment)
# =========================================================================
set(WINDOWS_DRIVERS gdi32 user32 kernel32 shell32 winmm)
set(CORE_GRAPHICS glfw3 opengl32 assimp)
set(AUDIO_ENGINE OpenAL32) # Swap to 'openal' lowercase if your specific build profile requires it

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${CORE_GRAPHICS}
    ${AUDIO_ENGINE}
    ${WINDOWS_DRIVERS}
)

message(STATUS " Build configured cleanly at Root Workspace. Output: ${PROJECT_NAME}.exe")
