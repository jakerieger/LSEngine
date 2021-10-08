#pragma once

#include "glad.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

#include <GLFW/glfw3.h>
#include <fmt/format.h>

#include <iostream>

class LSGUI
{
public:
  static bool DragFloatN_Colored(const char* label, float* v, int components, float v_speed, float v_min, float v_max, const char* display_format, float power = 1.0f);
  static bool ButtonColored(const char* label, ImVec4 color = ImGui::GetStyle().Colors[ImGuiCol_Button], const ImVec2 & size = ImVec2(0, 0));
  static bool Button(const char* label, const ImVec2& size = ImVec2(0, 0));
  static void InitImGUI(GLFWwindow* window, unsigned int glslVerMajor, unsigned int glslVerMinor);
  static void ApplyTheme();

  enum LS_SELECTABLE {
    LS_SELECTABLE_ENTITY,
    LS_SELECTABLE_MATERIAL,
    LS_SELECTABLE_MESH,
    LS_SELECTABLE_TEXTURE,
  };
};

