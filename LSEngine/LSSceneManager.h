#pragma once

#include "json.hpp"
#include "LSMaterial.h"
#include "LSEntity.h"
#include "LSUtilities.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

class LSSceneManager {
public:
  void LoadScene(const char* sceneFilePath);
  void SaveScene(const char* sceneFilePath, std::string sceneName, std::vector<LSEntity*> entities);

  std::vector<LSEntity*> entities;
  std::string sceneName;
};