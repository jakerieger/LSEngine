#pragma once

#include "glad.h"
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <algorithm>

#include "LSMaterial.h"

class LSEntity
{
public:
  unsigned int ID;
  glm::vec3 Position;
  glm::vec4 Rotation;
  glm::vec3 Scale;
  LSMaterial* Material;
  std::vector<float> Vertices;
  std::string Name;

  LSEntity(unsigned int ID = 0, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec4 rot = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
    std::vector<float> vert = {}, LSMaterial* mat = NULL, std::string name = "Object");
  ~LSEntity();

  void PrepBuffers();
  void Draw();
  void Cleanup();
private:
  unsigned int VBO, VAO;
};

