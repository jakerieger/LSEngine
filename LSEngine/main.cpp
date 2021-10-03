#include "LSEngine.h"

int main() {
  glm::vec4 clearColor = glm::vec4(
    5.0f / 255.0f,
    5.0f / 255.0f,
    8.0f / 255.0f,
    1.0f
  );

  LSEngine* engine = new LSEngine(1600, 900, "LSEngine", clearColor);

  try {
    engine->Run();
  }
  catch (std::exception e) {
    throw std::runtime_error(e.what());
  }
}