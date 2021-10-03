#pragma once

#include "glad.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>
#include <fmt/core.h>
#include <fmt/format.h>

#include "stb_image.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <time.h>

class LSUtilities
{
public:
  static void InitGLAD();
  static std::string GetTimestamp();
  static void SetWindowIcon(GLFWwindow* window, const char* iconPath);
  static std::string ReadFileToString(const char* filePath, const char* category);
  static int OutputLog(std::vector<std::string> logContents);

  class Shader {
  public:
    static void ShaderCheckCompile(unsigned int shader);
    static void ShaderCheckLink(unsigned int ID);
  };

  class GUI {
  public:
    static void InitImGUI(GLFWwindow* window, unsigned int glslVerMajor, unsigned int glslVerMinor);
    static void ApplyTheme();
  };
};

