#include "LSEntity.h"

LSEntity::LSEntity(unsigned int id, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, std::vector<float> vert, LSMaterial* mat, std::string name) {
  ID = id;
  Position = pos;
  Rotation = rot;
  Scale = scale;
  Material = mat;
  Vertices = vert;
  Name = name;
  enabled = true;
}

void LSEntity::PrepBuffers() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(float), &Vertices[0], GL_STATIC_DRAW);
  glBindVertexArray(VAO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  if (Material->diffuseTextureID != NULL) {
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
  }
}

void LSEntity::Draw() {
  Material->Bind(Position, Rotation, Scale);
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, (int)(Vertices.size() / 8));
  glBindVertexArray(0);
}

void LSEntity::Cleanup() {
  Material->Unbind();
  delete Material;

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
}