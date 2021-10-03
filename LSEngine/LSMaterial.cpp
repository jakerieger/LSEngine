#include "LSMaterial.h"

LSMaterial::LSMaterial(LS_SHADER_TYPES type, const char* dt, const char* st) {
  shader = new LSShader();
  shaderType = type;

  switch (shaderType) {
  case LS_MAT_PHONG:
    shader->LoadShaders("resources/shaders/phong.lsshader");
    break;
  case LS_MAT_UNLIT:
    shader->LoadShaders("resources/shaders/unlit.lsshader");
    break;
  case LS_MAT_DIFFUSE:
    shader->LoadShaders("resources/shaders/diffuse.lsshader");
    break;
  case LS_MAT_SPECULAR:
    shader->LoadShaders("resources/shaders/specular.lsshader");
    break;
  default:
    shader->LoadShaders("resources/shaders/unlit.lsshader");
    break;
  }

  if (dt != NULL) {
    diffuseTextureID = loadTexture(dt);
  }

  if (st != NULL) {
    specularTextureID = loadTexture(st);
  }
}

void LSMaterial::Bind(glm::vec3 position, glm::vec3 scale) {
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::scale(model, scale);
  model = glm::translate(model, position);

  shader->Use();
  shader->setMat4("projection", projection);
  shader->setMat4("view", view);
  shader->setMat4("model", model);

  switch (shaderType) {
  case LS_MAT_UNLIT:
    shader->setVec4("objectColor", objectColor);
    break;
  case LS_MAT_PHONG:
    shader->setVec4("objectColor", objectColor);
    shader->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    shader->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    shader->setVec3("light.position", 0.0f, 0.0f, 3.0f);

    if (diffuseTextureID != NULL) {
      shader->setInt("material.diffuse", 0);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, diffuseTextureID);
    }

    if (specularTextureID != NULL) {
      shader->setInt("material.specular", 1);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, specularTextureID);
    }
    break;
  default:
    break;
  }

  if (diffuseTextureID != NULL) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseTextureID);
  }

  if (specularTextureID != NULL) {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularTextureID);
  }
}

void LSMaterial::Unbind() {
  glDeleteProgram(shader->ID);
  delete shader;
}

std::string LSMaterial::GetMaterialTypeAsString() {
  switch (shaderType) {
  case LS_MAT_PHONG:
    return "Phong";
  case LS_MAT_UNLIT:
    return "Unlit";
  default:
    return "Unknown";
  }
}

LS_SHADER_TYPES LSMaterial::GetMaterialTypeAsEnum(const char* type)
{
  if (type == "Unlit") {
    return LS_MAT_UNLIT;
  }
  else if (type == "Phong") {
    return LS_MAT_PHONG;
  }
  else if (type == "Diffuse") {
    return LS_MAT_DIFFUSE;
  }
  else if (type == "Specular") {
    return LS_MAT_SPECULAR;
  }
  else {
    return LS_MAT_UNLIT;
  }
}

unsigned int LSMaterial::loadTexture(char const* path) {
  std::cout << "Loading Textures" << std::endl;

  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);

  if (data) {
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3) [[likely]]
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    std::cout << "Texture Loaded" << std::endl;
  }
  else {
    std::string msg = fmt::format("[!] Texture failed to load at path:{}\n", path);
    stbi_image_free(data);
    throw std::runtime_error(msg.c_str());
  }

  return textureID;
}
