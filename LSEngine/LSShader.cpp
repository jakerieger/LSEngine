#include "LSShader.h"

void LSShader::LoadShaders(const char* shaderPath) {
  std::string shaderCode;
  std::ifstream shaderFile;

  shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    shaderFile.open(shaderPath);
    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();
    shaderCode = shaderStream.str();
  }
  catch (std::ifstream::failure& e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
  }

  std::string vertexCode;
  std::string fragmentCode;

  int vertexTagLength = 9;
  int fragmentTagLength = 11;

  size_t vertStart = shaderCode.find("[VERTEX]");
  size_t vertEnd = shaderCode.find("[END VERTEX]");
  vertexCode = shaderCode.substr(vertStart + vertexTagLength, vertEnd - vertexTagLength);

  size_t fragStart = shaderCode.find("[FRAGMENT]");
  size_t fragEnd = shaderCode.find("[END FRAGMENT]");
  fragmentCode = shaderCode.substr(fragStart + fragmentTagLength);
  fragmentCode = fragmentCode.substr(0, fragmentCode.length() - 15);

  const char* c_vertexCode = vertexCode.c_str();
  const char* c_fragmentCode = fragmentCode.c_str();

  unsigned int vertex, fragment;

  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &c_vertexCode, NULL);
  glCompileShader(vertex);
  LSUtilities::Shader::ShaderCheckCompile(vertex);

  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &c_fragmentCode, NULL);
  glCompileShader(fragment);
  LSUtilities::Shader::ShaderCheckCompile(fragment);

  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  LSUtilities::Shader::ShaderCheckLink(ID);

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void LSShader::LoadShaders(const char* vertexPath, const char* fragmentPath) {
  std::string vertexCode;
  std::string fragmentCode;

  std::ifstream vShaderFile;
  std::ifstream fShaderFile;

  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    vShaderFile.close();
    fShaderFile.close();
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  }

  catch (std::ifstream::failure& e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
  }

  const char* vShaderCode = vertexCode.c_str();
  const char* fShaderCode = fragmentCode.c_str();

  unsigned int vertex, fragment;

  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  LSUtilities::Shader::ShaderCheckCompile(vertex);

  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);
  LSUtilities::Shader::ShaderCheckCompile(fragment);

  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  LSUtilities::Shader::ShaderCheckLink(ID);

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void LSShader::Use() const
{
  glUseProgram(ID);
}

void LSShader::setBool(const std::string& name, bool value) const
{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void LSShader::setInt(const std::string& name, int value) const
{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void LSShader::setFloat(const std::string& name, float value) const
{
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void LSShader::setVec2(const std::string& name, glm::vec2& value) const
{
  glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void LSShader::setVec2(const std::string& name, float x, float y) const
{
  glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void LSShader::setVec3(const std::string& name, glm::vec3& value) const
{
  glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void LSShader::setVec3(const std::string& name, float x, float y, float z) const
{
  glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void LSShader::setVec4(const std::string& name, glm::vec4& value) const
{
  glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void LSShader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
  glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void LSShader::setMat2(const std::string& name, const glm::mat2& mat) const
{
  glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void LSShader::setMat3(const std::string& name, const glm::mat3& mat) const
{
  glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void LSShader::setMat4(const std::string& name, const glm::mat4& mat) const
{
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}