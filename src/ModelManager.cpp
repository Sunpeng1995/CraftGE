#include "ModelManager.h"

ModelManager* ModelManager::gModelManager = nullptr;

ModelManager::ModelManager() {

}

ModelManager* ModelManager::getInstance() {
  if (gModelManager == nullptr) {
    gModelManager = new ModelManager();
  }
  return gModelManager;
}

bool ModelManager::isModelLoaded(std::string path) {
  auto search = mLoadedModels.find(path);
  if (search != mLoadedModels.end()) {
    return true;
  }
  return false;
}

void ModelManager::addModelData(std::string path, std::vector<shared_model_data*> data) {
  mLoadedModels[path] = data;
}