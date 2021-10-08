#include "LSEngine.h"

// GLOBALS
LSCamera camera;
float lastX, lastY;
bool firstMouse = true;
static int selectedItemID = -1;
LSGUI::LS_SELECTABLE selectedType;

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
bool Any(std::vector<std::string> test_cases, std::string test_string);

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

  LSUtilities::InitGLAD();
  LSGUI::InitImGUI(window, 4, 6);
  LSGUI::ApplyTheme();

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
  ImVec2 viewportSize_2 = ImVec2(viewport->Size.x, viewport->Size.y - ImGui::GetFrameHeight());

  ImGui::SetNextWindowPos(dockspaceArea);
  ImGui::SetNextWindowSize(viewportSize_2);
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
      std::string scenePath = rootDir + "scenes";

      const char* fileFilters[1] = { "*.lsscene" };

      const char* fd_result = tinyfd_openFileDialog(
        "Open Scene",
        scenePath.c_str(),
        1,
        fileFilters,
        NULL,
        0
      );

      if (fd_result != nullptr) {
        sceneManager.LoadScene(fd_result);
        entities = sceneManager.entities;
        std::string windowTitle = fmt::format("LSEngine (Development Build) | {} | <OpenGL 4.6>", sceneManager.sceneName);
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

#pragma region Scene Hierarchy
  ImGui::Begin("Scene Hierarchy");

  for (auto& entity : entities) {

    if (ImGui::Selectable(entity->Name.c_str(), selectedItemID == entity->ID)) {
      selectedItemID = entity->ID;
      selectedType = LSGUI::LS_SELECTABLE_ENTITY;
    }
  }

  ImGui::End();
#pragma endregion

#pragma region Inspector
  ImGui::Begin("Inspector");

  if (selectedItemID != -1) {
    UpdateInspector(selectedType);
  }

  ImGui::End();
#pragma endregion

#pragma region Content Browser
  ImGui::Begin("Content Browser");

  std::vector<std::string> imageExts = { ".png", ".jpg", ".jpeg", ".tif", ".bmp", ".gif", ".hdr" };
  std::string sceneExt = ".lsscene";
  std::string materialExt = ".lsmat";
  std::string shaderExt = ".lsshader";

  for (const auto& entry : std::filesystem::directory_iterator(rootDir)) {
    if (entry.is_directory()) {
      if (ImGui::TreeNode(entry.path().filename().string().c_str())) {
        bool is_selected = false;

        int i = 0;
        for (const auto& subEntry : std::filesystem::directory_iterator(entry.path())) {
          if (ImGui::Selectable(subEntry.path().filename().string().c_str(), is_selected)) {
            selectedItemID = i;

            if (Any(imageExts, subEntry.path().extension().string())) {
              selectedType = LSGUI::LS_SELECTABLE_TEXTURE;
            }
            else if (subEntry.path().extension().string() == materialExt)
              selectedType = LSGUI::LS_SELECTABLE_MATERIAL;
            else
              selectedItemID = -1;
          }

          i++;
        }

        ImGui::TreePop();
      }
    }
  }

  ImGui::End();
#pragma endregion

#pragma region Settings
  ImGui::Begin("Settings");

  if (ImGui::CollapsingHeader("Renderer")) {
    // V-SYNC
    ImGui::Checkbox("V-Sync", &EngineSettings.enableVsync);
  }

  if (ImGui::CollapsingHeader("Scene")) {
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
            // Formula: (y * width) / x = height
            // ex. 4:3 aspect ratio => (3 * width) / 4
          }
            
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
      }

      ImGui::TreePop();
    }
  }

  ImGui::End();
#pragma endregion

#pragma region Analytics
  ImGui::Begin("Analytics");

  char* renderer = (char*)glGetString(GL_RENDERER);
  std::string rendererS;
  rendererS += renderer;
  std::string gpuInfo = fmt::format("GPU: {}", rendererS);

  ImGui::Text("GPU");
  ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - ImGui::CalcTextSize(renderer).x);
  ImGui::Text(renderer);
  ImGui::Separator();

  ImGui::Text("Framerate");
  std::string framerate = fmt::format("{:.{}f}", ImGui::GetIO().Framerate, 1);
  ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - ImGui::CalcTextSize(framerate.c_str()).x);

  if (ImGui::GetIO().Framerate >= 60.0f) {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4({ 0.0f, 1.0f, 0.0f, 1.0f }));
    ImGui::Text("%.1f", ImGui::GetIO().Framerate);
    ImGui::PopStyleColor();
  }
  else if (ImGui::GetIO().Framerate < 60.0f && ImGui::GetIO().Framerate > 30.0f) {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4({ 1.0f, 0.8f, 0.0f, 1.0f }));
    ImGui::Text("%.1f", ImGui::GetIO().Framerate);
    ImGui::PopStyleColor();
  }
  else {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4({ 1.0f, 0.0f, 0.0f, 1.0f }));
    ImGui::Text("%.1f", ImGui::GetIO().Framerate);
    ImGui::PopStyleColor();
  }

  ImGui::Separator();

  ImGui::Text("Time Per Frame");
  std::string timeperframe = fmt::format("{:.{}f}ms", 1000.0f / ImGui::GetIO().Framerate, 1);
  ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - ImGui::CalcTextSize(timeperframe.c_str()).x);

  if (ImGui::GetIO().Framerate >= 60.0f) {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4({ 0.0f, 1.0f, 0.0f, 1.0f }));
    ImGui::Text("%.1fms", 1000.0f / ImGui::GetIO().Framerate);
    ImGui::PopStyleColor();
  }
  else if (ImGui::GetIO().Framerate < 60.0f && ImGui::GetIO().Framerate > 30.0f) {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4({ 1.0f, 0.8f, 0.0f, 1.0f }));
    ImGui::Text("%.1fms", 1000.0f / ImGui::GetIO().Framerate);
    ImGui::PopStyleColor();
  }
  else {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4({ 1.0f, 0.0f, 0.0f, 1.0f }));
    ImGui::Text("%.1fms", 1000.0f / ImGui::GetIO().Framerate);
    ImGui::PopStyleColor();
  }

  ImGui::Separator();

  ImGui::Text("VRAM Usage");

  float memoryUsage = std::get<0>(LSUtilities::GetGPUMemoryUsage());
  float availMemory = std::get<2>(LSUtilities::GetGPUMemoryUsage());

  std::string memusage = fmt::format("{:.{}f}MB", memoryUsage, 1);
  ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - ImGui::CalcTextSize(memusage.c_str()).x);

  if (memoryUsage / availMemory <= 0.25f) {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4({ 0.0f, 1.0f, 0.0f, 1.0f }));
    ImGui::Text("%.1fMB", memoryUsage);
    ImGui::PopStyleColor();
  }
  else if (memoryUsage / availMemory > 0.25f && memoryUsage / availMemory <= 0.5f) {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4({ 1.0f, 0.8f, 0.0f, 1.0f }));
    ImGui::Text("%.1fMB", memoryUsage);
    ImGui::PopStyleColor();
  }
  else {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4({ 1.0f, 0.0f, 0.0f, 1.0f }));
    ImGui::Text("%.1fMB", memoryUsage);
    ImGui::PopStyleColor();
  }

  ImGui::Separator();
  ImGui::Text("Available VRAM");
  std::string availmem = fmt::format("{:.{}f}MB", availMemory, 1);
  ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - ImGui::CalcTextSize(availmem.c_str()).x);
  ImGui::Text("%.1fMB", availMemory);

  ImGui::End();
#pragma endregion

#pragma region Toolbox
  ImGui::Begin("##Toolbox");

  if (LSGUI::Button("Wireframe")) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  ImGui::SameLine();

  if (LSGUI::Button("Fill")) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  ImGui::SameLine();

  if (LSGUI::Button("Load Scene")) {
    sceneManager.LoadScene("resources/scenes/test.lsscene");
    entities = sceneManager.entities;

    std::string windowTitle = fmt::format("LSEngine (Development Build) | Renderer: OpenGL | Scene: {}", sceneManager.sceneName);
    glfwSetWindowTitle(window, windowTitle.c_str());
  }

  ImGui::SameLine();

  if (LSGUI::Button("Save Scene")) {
    sceneManager.SaveScene("resources/scenes/demo.lsscene", "Demo", entities);
  }

  ImGui::SameLine();

  if (LSGUI::Button("Add Cube")) {
    LSMaterial* mat = new LSMaterial(LS_MAT_UNLIT, NULL, NULL, "cubemat");
    Cube* cube = new Cube();

    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rot = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

    unsigned int numEntities = entities.size();
    std::string entityName = fmt::format("Object {}", numEntities);

    LSEntity* entity = new LSEntity(numEntities, pos, rot, scale, cube->vertices, mat, entityName);

    entity->PrepBuffers();

    entities.push_back(entity);

    Log("Added primitive [cube]");
  }

  ImGui::SameLine();

  if (LSGUI::Button("Add Light")) {
    LSMaterial* mat = new LSMaterial(LS_MAT_UNLIT, NULL, NULL, "lightmat");
    mat->objectColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    Cube* cube = new Cube();
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rot = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(0.2f, 0.2f, 0.2f);

    unsigned int numEntities = entities.size();
    std::string entityName = fmt::format("Object {}", numEntities);

    LSEntity* entity = new LSEntity(numEntities, pos, rot, scale, cube->vertices, mat, entityName);
    entities.push_back(entity);

    Log("Added light");
  }

  ImGui::SameLine();

  if (LSGUI::Button("Export Log")) {
    int result = LSUtilities::OutputLog(DebugLog);

    if (result == 0) {
      Log("Debug log exported successfully!");
    }
    else {
      Log("Debug log export failed!");
    }
  }

  ImGui::End();
#pragma endregion

#pragma region Log
  ImGui::Begin("Log");

  for (std::string logEntry : DebugLog) {
    ImGui::Text(logEntry.c_str());
  }

  ImGui::End();
#pragma endregion

#pragma region Scene
  ImGui::Begin("Scene");
  ImGui::BeginChild("SceneRender");
  ImVec2 viewportSize = ImGui::GetWindowSize();

  viewportWidth = viewportSize.x;
  viewportHeight = viewportSize.y;

  ImGui::Image((ImTextureID)renderTexID, viewportSize, ImVec2(0, 1), ImVec2(1, 0));
  ImGui::EndChild();
  ImGui::End();
#pragma endregion

  ImGui::Render();
}

void LSEngine::UpdateInspector(LSGUI::LS_SELECTABLE selectableType)
{
  if (selectableType == LSGUI::LS_SELECTABLE_ENTITY) {
    LSEntity* entity = entities[selectedItemID];

    ImGui::Checkbox("##entityEnabled", &entities[selectedItemID]->enabled);
    ImGui::SameLine();
    // Entity name InputText
    ImGui::Text(entities[selectedItemID]->Name.c_str());
    ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - 32.0f);
    if (LSGUI::ButtonColored("Delete", ImVec4({1.0f, 0.22f, 0.38f, 1.0f}))) {
      entities.erase(entities.begin() + entities[selectedItemID]->ID);
      for (int i = 0; i < entities.size(); i++) {
        entities[i]->ID = i;
      }
      selectedItemID = -1;
    }

    // Transform
    if (ImGui::CollapsingHeader("Transform")) {
      ImGui::Text("Position");
      ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - ImGui::CalcItemWidth());
      std::string posTag = fmt::format("##position{}", selectedItemID);
      LSGUI::DragFloatN_Colored(posTag.c_str(), (float*)&entity->Position, 3, 0.1f, -10.0f, 10.0f, "%.1f");

      ImGui::Text("Rotation");
      ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - ImGui::CalcItemWidth());
      std::string rotTag = fmt::format("##rotation{}", selectedItemID);
      LSGUI::DragFloatN_Colored(rotTag.c_str(), (float*)&entity->Rotation, 3, 0.5f, -180.0f, 180.0f, "%.1f");

      ImGui::Text("Scale");
      ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - ImGui::CalcItemWidth());
      std::string sclTag = fmt::format("##scale{}", selectedItemID);
      LSGUI::DragFloatN_Colored(sclTag.c_str(), (float*)&entity->Scale, 3, 0.1f, 0.0001f, 1000.0f, "%.1f");
    }

    // Material
    std::string materialHdr = fmt::format("Material ({})", entity->Material->Name);
    if (ImGui::CollapsingHeader(materialHdr.c_str())) {
      ImGui::Text("Shader");
      ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - ImGui::CalcTextSize(entity->Material->GetMaterialTypeAsString().c_str()).x);
      ImGui::Text(entity->Material->GetMaterialTypeAsString().c_str());

      ImGui::Text("Base Color");
      ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - ImGui::CalcItemWidth());
      std::string colorTag = fmt::format("##color{}", entity->ID);

      ImGui::ColorPicker4(colorTag.c_str(), (float*)&entity->Material->objectColor);
    }

    ImGui::Separator();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() * 0.5f);
    if (LSGUI::Button("Add Component", ImVec2(-1.0f, 0.0f))) {
      //
    }
  }
  else if (selectableType == LSGUI::LS_SELECTABLE_MATERIAL) {

  }
  else if (selectableType == LSGUI::LS_SELECTABLE_MESH) {

  }
  else if (selectableType == LSGUI::LS_SELECTABLE_TEXTURE) {
    if (Resources.textures[selectedItemID] != selectedTextureCache) {
      std::cout << "Loading texture preview data" << std::endl;
      auto future = std::async(LSUtilities::LoadTextureThreaded, Resources.textures[selectedItemID].c_str(), &image_width, &image_height);
      auto image_data = future.get();
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
      stbi_image_free(image_data);
      selectedTextureCache = Resources.textures[selectedItemID];
    }

    ImVec2 previewSize = {
      ImGui::GetContentRegionAvailWidth(),
      (image_height * ImGui::GetContentRegionAvailWidth()) / image_width
    };

    ImGui::Image((ImTextureID)texture_preview, previewSize);
    //ImGui::End();
  }
  else {

  }
}

void LSEngine::RenderLoop() {
  glEnable(GL_DEPTH_TEST);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //glEnable(GL_BLEND);

  /*
  LSShader* gridShader = new LSShader();
  gridShader->LoadShaders("resources/shaders/grid.lsshader");
  */

  LSMaterial* material2 = new LSMaterial(LS_MAT_UNLIT, NULL, NULL, "test material");

  Cube* myCube = new Cube();

  LSEntity* myEntity2 = new LSEntity(entities.size(), glm::vec3(3.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), myCube->vertices, material2, "my cube 2");
  Log(fmt::format("Added entity: {}", myEntity2->Name));
  entities.push_back(myEntity2);

  camera.Use(glm::vec3(0.0f, 1.0f, 3.0f));

  /*
  unsigned int gridVAO;
  glGenVertexArrays(1, &gridVAO);
  glBindVertexArray(gridVAO);
  */

  for (auto& entity : entities) {
    entity->PrepBuffers();
  }

  float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
  };

  unsigned int skyboxVAO, skyboxVBO;
  glGenVertexArrays(1, &skyboxVAO);
  glGenBuffers(1, &skyboxVBO);
  glBindVertexArray(skyboxVAO);
  glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  std::vector<std::string> cubemapFaces = {
  "resources/textures/skybox/right.jpg",
  "resources/textures/skybox/left.jpg",
  "resources/textures/skybox/top.jpg",
  "resources/textures/skybox/bottom.jpg",
  "resources/textures/skybox/front.jpg",
  "resources/textures/skybox/back.jpg",
  };

  unsigned int skyboxTex = LSUtilities::LoadCubemap(cubemapFaces);
  std::cout << skyboxTex << std::endl;

  LSShader* skyboxShader = new LSShader();
  skyboxShader->LoadShaders("resources/shaders/skybox.lsshader");
  skyboxShader->Use();
  skyboxShader->setInt("skybox", 0);

#pragma region Scene FBO
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
#pragma endregion

  glGenTextures(1, &texture_preview);
  glBindTexture(GL_TEXTURE_2D, texture_preview);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  while (!glfwWindowShouldClose(window)) {
    load_resources();

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

    /*
    switch (EngineSettings.enableGrid) {
    case true:
      gridShader->Use();
      gridShader->setMat4("view", view);
      gridShader->setMat4("proj", projection);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      break;
    case false:
      glDepthFunc(GL_LESS);
    }
    */

    glDepthFunc(GL_LESS);

    for (auto& entity : entities) {
      entity->Material->projection = projection;
      entity->Material->view = view;
      entity->Material->shader->setVec3("viewPos", camera.Position);

      if (entity->enabled)
        entity->Draw();
    }

    //glDepthFunc(GL_LEQUAL);
    skyboxShader->Use();
    view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
    skyboxShader->setMat4("view", view);
    skyboxShader->setMat4("projection", projection);
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    //glDepthFunc(GL_LESS); // set depth function back to default

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glBindTexture(GL_TEXTURE_2D, 0);
  glDeleteTextures(1, &texture_preview);

  for (auto& entity : entities) {
    entity->Cleanup();
  }

  glDeleteVertexArrays(1, &skyboxVAO);
  glDeleteBuffers(1, &skyboxVAO);
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

void LSEngine::load_resources()
{
  // Update Resources struct
  Resources.textures = {};
  for (const auto& entry : std::filesystem::directory_iterator(rootDir + "textures\\")) {
    if (entry.is_regular_file()) {
      Resources.textures.push_back(entry.path().string());
    }
  }
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

bool Any(std::vector<std::string> test_cases, std::string test_string) {
  for (int i = 0; i < test_cases.size(); i++) {
    if (test_string.find(test_cases[i]) != std::string::npos)
      return true;
  }

  return false;
}