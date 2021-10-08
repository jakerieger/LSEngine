#pragma once

#include "glad.h"
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
#include <thread>
#include <future>

class LSUtilities
{
public:
  static void InitGLAD();
  static std::string GetTimestamp();
  static void SetWindowIcon(GLFWwindow* window, const char* iconPath);
  static std::string ReadFileToString(const char* filePath, const char* category);
  static int OutputLog(std::vector<std::string> logContents);
  static bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
  static std::tuple<float, float, float> GetGPUMemoryUsage();
  static unsigned char* LoadTextureThreaded(const char* filename, int* image_width, int* image_height);
  static unsigned int LoadCubemap(std::vector<std::string> faces);

  class Shader {
  public:
    static void ShaderCheckCompile(unsigned int shader);
    static void ShaderCheckLink(unsigned int ID);
  };
};

