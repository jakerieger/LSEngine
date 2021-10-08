#include "LSSceneManager.h"

using json = nlohmann::json;

void LSSceneManager::LoadScene(const char* sceneFilePath) {
  entities = {}; // init empty entities vector

  std::string sceneData = LSUtilities::ReadFileToString(sceneFilePath, "SCENE");
  json sceneJSON = json::parse(sceneData);

  std::string _sceneName = sceneJSON["name"];
  sceneName = _sceneName;

  for (auto& entity : sceneJSON["entities"]) {
    glm::vec3 pos = glm::vec3(
      entity["transform"]["position"]["x"],
      entity["transform"]["position"]["y"],
      entity["transform"]["position"]["z"]
    );

    glm::vec3 rot = glm::vec3(
      entity["transform"]["rotation"]["x"],
      entity["transform"]["rotation"]["y"],
      entity["transform"]["rotation"]["z"]
    );

    glm::vec3 scale = glm::vec3(
      entity["transform"]["scale"]["x"],
      entity["transform"]["scale"]["y"],
      entity["transform"]["scale"]["z"]
    );

    std::string matType = entity["materialType"];
    LSMaterial* mat = new LSMaterial(LSMaterial::GetMaterialTypeAsEnum(matType.c_str()), nullptr, nullptr, "mat_name_here");
    mat->objectColor = glm::vec4(
      entity["objectColor"]["R"],
      entity["objectColor"]["G"],
      entity["objectColor"]["B"],
      entity["objectColor"]["A"]
    );

    std::vector<float> verts;
    for (auto& vert : entity["geometry"]) {
      verts.push_back(vert);
    }

    // TODO: Add Material Textures

    std::string entityName = entity["name"];
    LSEntity* entity = new LSEntity(entities.size(), pos, rot, scale, verts, mat, entityName);

    entity->PrepBuffers();

    entities.push_back(entity);
  }
}

void LSSceneManager::SaveScene(const char* sceneFilePath, std::string sceneName, std::vector<LSEntity*> entities) {
  json sceneData;

  sceneData["name"] = sceneName;
  sceneData["entities"] = json::array();

  int i = 0;
  for (auto& entity : entities) {
    sceneData["entities"][i]["name"] = entity->Name;
    sceneData["entities"][i]["materialType"] = entity->Material->GetMaterialTypeAsString();
    sceneData["entities"][i]["objectColor"]["R"] = entity->Material->objectColor.x;
    sceneData["entities"][i]["objectColor"]["G"] = entity->Material->objectColor.y;
    sceneData["entities"][i]["objectColor"]["B"] = entity->Material->objectColor.z;
    sceneData["entities"][i]["objectColor"]["A"] = entity->Material->objectColor.w;
    sceneData["entities"][i]["geometry"] = entity->Vertices;
    sceneData["entities"][i]["transform"]["position"]["x"] = entity->Position.x;
    sceneData["entities"][i]["transform"]["position"]["y"] = entity->Position.y;
    sceneData["entities"][i]["transform"]["position"]["z"] = entity->Position.z;
    sceneData["entities"][i]["transform"]["rotation"]["x"] = entity->Rotation.x;
    sceneData["entities"][i]["transform"]["rotation"]["y"] = entity->Rotation.y;
    sceneData["entities"][i]["transform"]["rotation"]["z"] = entity->Rotation.z;
    sceneData["entities"][i]["transform"]["scale"]["x"] = entity->Scale.x;
    sceneData["entities"][i]["transform"]["scale"]["y"] = entity->Scale.y;
    sceneData["entities"][i]["transform"]["scale"]["z"] = entity->Scale.z;

    i++;
  }

  std::string sceneData_str = sceneData.dump(2);
  std::string filepath = fmt::format("resources/scenes/{}.lsscene", sceneName);

  try {
    std::ofstream out(filepath.c_str());
    out << sceneData_str;
    out.close();
  }
  catch (std::ofstream::failure& e) {
    std::cout << "ERROR::SCENE::COULD_NOT_SAVE" << std::endl;
  }
}