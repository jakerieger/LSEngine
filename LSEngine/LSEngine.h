#pragma once

#include <stdexcept>
#include <iostream>

#include "glad.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "tinyfiledialogs.h"

#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <fmt/format.h>

#include "LSShader.h"
#include "LSCamera.h"
#include "LSPrimative.h"
#include "LSEntity.h"
#include "LSSceneManager.h"
#include "LSUtilities.h"

#include <algorithm>
#include <vector>
#include <iterator>
#include <chrono>
#include <filesystem>

#define PI 3.1415926535897932384626433832795

struct T_ENGINE_SETTINGS {
  bool enableVsync = false;
  bool enableGrid = true;
  glm::vec2 aspectRatio;
};

class LSEngine
{
public:
  LSEngine(int width, int height, const char* title, glm::vec4 clearColor);
  ~LSEngine();

  void Run();

  int WIDTH, HEIGHT;
  const char* TITLE;

  glm::vec4 CLEARCOLOR;

private:
  GLFWwindow* window;
  LSShader* gridShader;
  LSSceneManager sceneManager;

  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  bool isRightClick = false;

  std::vector<LSEntity*> entities;
  std::vector<std::string> DebugLog;

  float viewportWidth, viewportHeight;

  T_ENGINE_SETTINGS EngineSettings;

  void InitWindow();
  void DrawGUI(unsigned int renderTexID);
  void RenderLoop();
  void Log(std::string message, bool hasTimestamp = true);
  void ProcessInput(GLFWwindow* window);
  void Cleanup();
};

