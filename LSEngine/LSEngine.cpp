#include "LSEngine.h"

// TODO: implement these as class member variables instead of global variables
LSCamera camera;
float lastX, lastY;
bool firstMouse = true;

// Global functions used for OpenGL / GLFW callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void GLAPIENTRY MessageCallback(GLenum source, 
  GLenum type, 
  GLuint id, 
  GLenum severity, 
  GLsizei length, 
  const char* message,
  const void* userParam);

LSEngine::LSEngine(int width, int height, const char* title, glm::vec4 clearColor) {
  WIDTH = width;
  HEIGHT = height;
  TITLE = title;
  CLEARCOLOR = clearColor;
}

LSEngine::~LSEngine() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void LSEngine::Run() {
  InitWindow();
  RenderLoop();
  Cleanup();
}

void LSEngine::InitWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
  if (window == NULL) {
    glfwTerminate();
    throw std::runtime_error("[!] Failed to create GLFW window!\n");
  }

  glfwMakeContextCurrent(window);

  // Enabled V-SYNC
  // glfwSwapInterval(1);

  LSUtilities::InitGLAD();
  LSUtilities::GUI::InitImGUI(window, 4, 6);
  LSUtilities::GUI::ApplyTheme();

  glViewport(0, 0, WIDTH, HEIGHT);
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(MessageCallback, 0);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetScrollCallback(window, scroll_callback);

  LSUtilities::SetWindowIcon(window, "resources/images/logo.png");

  Log("** LSEngine Initialized **", false);
}

void LSEngine::DrawGUI(unsigned int renderTexID) {
  ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

  ImGuiViewport* viewport = ImGui::GetMainViewport();

  ImVec2 dockspaceArea = ImVec2(viewport->Pos.x, viewport->Pos.y + ImGui::GetFrameHeight());

  ImGui::SetNextWindowPos(dockspaceArea);
  ImGui::SetNextWindowSize(viewport->Size);
  ImGui::SetNextWindowViewport(viewport->ID);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("InvisibleWindow", nullptr, windowFlags);
  ImGui::PopStyleVar(3);

  ImGuiID dockSpaceID = ImGui::GetID("InvisibleWindowDockSpace");
  ImGui::DockSpace(dockSpaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

  ImGui::BeginMainMenuBar();
  if (ImGui::BeginMenu("File"))
  {
    if (ImGui::MenuItem("New Scene")) {

    }

    if (ImGui::MenuItem("Open Scene", "Ctrl+O")) {
      std::filesystem::path cwd = std::filesystem::current_path();
      std::string workingDir = cwd.string();

      const char* fileFilters[1] = { "*.lsscene" };

      const char* fd_result = tinyfd_openFileDialog(
        "Open Scene",
        workingDir.c_str(),
        1,
        fileFilters,
        NULL,
        0
      );

      if (fd_result != nullptr) {
        sceneManager.LoadScene(fd_result);
        entities = sceneManager.entities;
        std::string windowTitle = fmt::format("LSEngine (Development Build) | Renderer: OpenGL | Scene: {}", sceneManager.sceneName);
        glfwSetWindowTitle(window, windowTitle.c_str());
      }
    }

    if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {

    }

    if (ImGui::MenuItem("Save Scene As", "Ctrl+Shift+S")) {

    }

    ImGui::EndMenu();
  }

  ImGui::EndMainMenuBar();

  ImGui::End();

  ImGui::Begin("Scene Overview");

  for (auto& entity : entities) {
    if (ImGui::TreeNode(entity->Name.c_str())) {
      if (ImGui::TreeNode("Transform")) {
        std::string positionLabel = fmt::format("Position [{}]", entity->Name);
        std::string scaleLabel = fmt::format("Scale [{}]", entity->Name);

        ImGui::SliderFloat3(positionLabel.c_str(), (float*)&entity->Position, -10.0f, 10.0f, "%.1f");
        ImGui::SliderFloat3(scaleLabel.c_str(), (float*)&entity->Scale, 0.0f, 10.0f, "%.1f");
        ImGui::TreePop();
      }

      if (ImGui::TreeNode("Material")) {
        std::string materialType = fmt::format("Type: {}", entity->Material->GetMaterialTypeAsString());
        ImGui::Text(materialType.c_str());

        std::string materialColorLbl = fmt::format("Color [{}]", (char)entity->ID);
        ImGui::ColorPicker4(materialColorLbl.c_str(), (float*)&entity->Material->objectColor);

        ImGui::TreePop();
      }

      if (ImGui::Button("Delete Entity")) {
        // Remove entity from scene
        entities.erase(entities.begin() + entity->ID);

        // Update remaining entity IDs
        for (int i = 0; i < entities.size(); i++) {
          entities[i]->ID = i;
        }
      }

      ImGui::TreePop();
    }
  }

  ImGui::End();

  // ImGui::Begin("Inspector");
  // ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

  // for (auto& entity : entities) {

  // }

  // ImGui::End();

  ImGui::Begin("Content Browser");
  ImGui::End();

  ImGui::Begin("Settings");

  if (ImGui::TreeNode("Renderer")) {
    // V-SYNC
    ImGui::Checkbox("V-Sync", &EngineSettings.enableVsync);
    ImGui::TreePop();
  }

  if (ImGui::TreeNode("Scene")) {
    // Reference Grid
    ImGui::Checkbox("Grid", &EngineSettings.enableGrid);

    if (ImGui::TreeNode("Camera")) {
      ImGui::SliderFloat("FOV", &camera.Zoom, 0.0f, 120.0f, "%0.1f");
      ImGui::SliderFloat("Movement Speed", &camera.MovementSpeed, 0.0f, 10.0f, "%.1f");
      ImGui::Text("Pitch: %.1f Degrees", camera.Pitch);
      ImGui::Text("Yaw: %.1f Degrees", camera.Yaw);

      const char* aspectRatioOptions[] = { "Custom", "16:9", "16:10", "4:3", "1:1" };
      static const char* current_item = aspectRatioOptions[0];

      ImGui::PushItemWidth(0.5f * ImGui::GetWindowContentRegionWidth());
      if (ImGui::BeginCombo("Aspect Ratio", current_item)) {
        for (int n = 0; n < IM_ARRAYSIZE(aspectRatioOptions); n++) {
          bool is_selected = (current_item == aspectRatioOptions[n]);
          if (ImGui::Selectable(aspectRatioOptions[n], is_selected)) {
            current_item = aspectRatioOptions[n];

            // Change Aspect Ratio
            viewportHeight = 2.0f;
          }
            
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
      }

      ImGui::TreePop();
    }

    ImGui::TreePop();
  }

  ImGui::End();

  ImGui::Begin("Inspector");
  ImGui::End();

  ImGui::Begin("Analytics");

  char* renderer = (char*)glGetString(GL_RENDERER);
  std::string rendererS;
  rendererS += renderer;
  std::string gpuInfo = fmt::format("GPU: {}", rendererS);
  ImGui::Text(gpuInfo.c_str());
  ImGui::Text("FPS: %.01f", ImGui::GetIO().Framerate);
  ImGui::Text("Time Per Frame: %.0001fms", 1000.0f / ImGui::GetIO().Framerate);

  ImGui::End();

  ImGui::Begin("Toolbox");

  if (ImGui::Button("Wireframe")) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  ImGui::SameLine();

  if (ImGui::Button("Fill")) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  ImGui::SameLine();

  if (ImGui::Button("Load Scene")) {
    sceneManager.LoadScene("resources/scenes/test.lsscene");
    entities = sceneManager.entities;

    std::string windowTitle = fmt::format("LSEngine (Development Build) | Renderer: OpenGL | Scene: {}", sceneManager.sceneName);
    glfwSetWindowTitle(window, windowTitle.c_str());
  }

  ImGui::SameLine();

  if (ImGui::Button("Save Scene")) {
    sceneManager.SaveScene("resources/scenes/demo.lsscene", "Demo", entities);
  }

  ImGui::SameLine();

  if (ImGui::Button("Add Cube")) {
    LSMaterial* mat = new LSMaterial(LS_MAT_UNLIT, NULL, NULL);
    Cube* cube = new Cube();

    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec4 rot = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

    unsigned int numEntities = entities.size();
    std::string entityName = fmt::format("Object {}", numEntities);

    LSEntity* entity = new LSEntity(numEntities, pos, rot, scale, cube->vertices, mat, entityName);
    entities.push_back(entity);

    Log("Added primitive [cube]");
  }

  ImGui::SameLine();

  if (ImGui::Button("Add Light")) {
    LSMaterial* mat = new LSMaterial(LS_MAT_UNLIT, NULL, NULL);
    mat->objectColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    Cube* cube = new Cube();
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec4 rot = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(0.2f, 0.2f, 0.2f);

    unsigned int numEntities = entities.size();
    std::string entityName = fmt::format("Object {}", numEntities);

    LSEntity* entity = new LSEntity(numEntities, pos, rot, scale, cube->vertices, mat, entityName);
    entities.push_back(entity);

    Log("Added light");
  }

  ImGui::SameLine();

  if (ImGui::Button("Export Log")) {
    int result = LSUtilities::OutputLog(DebugLog);

    if (result == 0) {
      Log("Debug log exported successfully!");
    }
    else {
      Log("Debug log export failed!");
    }
  }

  ImGui::End();

  ImGui::Begin("Log");

  for (std::string logEntry : DebugLog) {
    ImGui::Text(logEntry.c_str());
  }

  ImGui::End();

  // if (ImGui::BeginMainMenuBar()) {
  //   if (ImGui::BeginMenu("File")) {
  //     if (ImGui::MenuItem("New")) {
  //       //
  //     }
  //     ImGui::EndMenu();
  //   }

  //   ImGui::EndMainMenuBar();
  // }

  ImGui::Begin("Scene");
  ImGui::BeginChild("SceneRender");
  ImVec2 wsize = ImGui::GetWindowSize();

  viewportWidth = wsize.x;
  viewportHeight = wsize.y;

  ImGui::Image((ImTextureID)renderTexID, wsize, ImVec2(0, 1), ImVec2(1, 0));
  ImGui::EndChild();
  ImGui::End();

  ImGui::Render();
}

void LSEngine::RenderLoop() {
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  LSShader* gridShader = new LSShader();
  gridShader->LoadShaders("resources/shaders/grid.lsshader");

  //LSMaterial* material = new LSMaterial(LS_MAT_PHONG, "resources/textures/container2.png", "resources/textures/container2_specular.png");
  //material->objectColor = glm::vec4(20.f / 255.f, 90.f / 255.f, 123.f / 255.f, 1.0f);

  LSMaterial* material2 = new LSMaterial(LS_MAT_UNLIT, NULL, NULL);

  Cube* myCube = new Cube();

  //LSEntity* myEntity = new LSEntity(entities.size(), glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), myCube->vertices, material, "my cube");
  //Log(fmt::format("Added entity: {}", myEntity->Name));
  //entities.push_back(myEntity);


  LSEntity* myEntity2 = new LSEntity(entities.size(), glm::vec3(3.0f, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), myCube->vertices, material2, "my cube 2");
  Log(fmt::format("Added entity: {}", myEntity2->Name));
  entities.push_back(myEntity2);

  camera.Use(glm::vec3(0.0f, 1.0f, 3.0f));

  unsigned int gridVAO;
  glGenVertexArrays(1, &gridVAO);
  glBindVertexArray(gridVAO);

  for (auto& entity : entities) {
    entity->PrepBuffers();
  }

  unsigned int framebuffer;
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

  unsigned int textureColorbuffer;
  glGenTextures(1, &textureColorbuffer);
  glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

  auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "Framebuffer not complete: " << fboStatus << std::endl;

  if (fboStatus == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
    std::cout << "Incomplete attachment" << std::endl;

  if (fboStatus == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
    std::cout << "Incomplete missing attachment" << std::endl;

  if (fboStatus == GL_FRAMEBUFFER_UNSUPPORTED)
    std::cout << "Unsupported" << std::endl;

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  while (!glfwWindowShouldClose(window)) {
    switch (EngineSettings.enableVsync) {
    case true:
      glfwSwapInterval(1);
      break;
    case false:
      glfwSwapInterval(0);
      break;
    }

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    ProcessInput(window);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    DrawGUI(textureColorbuffer);

    glClearColor(CLEARCOLOR.x, CLEARCOLOR.y, CLEARCOLOR.z, CLEARCOLOR.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), viewportWidth / viewportHeight, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    switch (EngineSettings.enableGrid) {
    case true:
      glDepthFunc(GL_LEQUAL);
      gridShader->Use();
      gridShader->setMat4("view", view);
      gridShader->setMat4("proj", projection);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glDepthFunc(GL_LESS);
      break;
    case false:
      glDepthFunc(GL_LESS);
    }

    for (auto& entity : entities) {
      entity->Material->projection = projection;
      entity->Material->view = view;
      entity->Material->shader->setVec3("viewPos", camera.Position);
      entity->Draw();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  for (auto& entity : entities) {
    entity->Cleanup();
  }
}

void LSEngine::ProcessInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);

  if (glfwGetMouseButton(window, 1) == GLFW_PRESS && !isRightClick) {
    isRightClick = true;
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }
  else if (glfwGetMouseButton(window, 1) == GLFW_RELEASE) {
    isRightClick = false;
    glfwSetCursorPosCallback(window, NULL);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
}

void LSEngine::Log(std::string message, bool hasTimestamp) {
  if (hasTimestamp) {
    auto time = LSUtilities::GetTimestamp();
    std::string logoutput = fmt::format("[{}] {}", time, message);
    DebugLog.push_back(logoutput);
  }
  else {
    DebugLog.push_back(message);
  }
}

void LSEngine::Cleanup() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(yoffset);
}

void GLAPIENTRY MessageCallback(GLenum source,
  GLenum type,
  GLuint id,
  GLenum severity,
  GLsizei length,
  const char* message,
  const void* userParam)
{
  if ((severity == GL_DEBUG_SEVERITY_HIGH) || (severity == GL_DEBUG_SEVERITY_MEDIUM)) {
    std::string errorOutput = fmt::format("[ERROR] ID: {} | Message: {}", id, message);
    std::cout << errorOutput << std::endl;
  }
}