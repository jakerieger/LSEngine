#pragma once

#include "glad.h"
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <fmt/format.h>

#include "stb_image.h"
#include "LSShader.h"

#include <iostream>

struct T_MATERIAL_PROPS {
  glm::vec3 diffuseColor;
  glm::vec3 specularColor;
  float diffuseStrength;
  float specularStrength;
  float shininess;
};

struct T_PBR_MATERIAL_PROPS {
  float albedo;
};

enum LS_SHADER_TYPES {
  LS_MAT_PHONG,
  LS_MAT_UNLIT,
  LS_MAT_DIFFUSE,
  LS_MAT_SPECULAR,
  LS_MAT_ALPHA,
  LS_MAT_PBR,
  LS_MAT_PBR_UNLIT
};

class LSMaterial {
public:
  LSShader* shader;
  unsigned int diffuseTextureID;
  unsigned int specularTextureID;
  T_MATERIAL_PROPS materialProperties;
  T_PBR_MATERIAL_PROPS PBRmaterialProperties;
  glm::mat4 projection;
  glm::mat4 view;
  bool isPBR;
  LS_SHADER_TYPES shaderType;
  glm::vec4 objectColor = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);


  LSMaterial(LS_SHADER_TYPES type, const char* dt, const char* st);

  std::string GetMaterialTypeAsString();
  static LS_SHADER_TYPES GetMaterialTypeAsEnum(const char* type);
  void Bind(glm::vec3 position, glm::vec3 scale);
  void Unbind();

private:
  unsigned int loadTexture(char const* path);
};