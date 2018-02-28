#ifndef __MODEL_MANAGER_H__
#define __MODEL_MANAGER_H__

#include <unordered_map>
#include "Texture.h"

struct shared_model_data {
  GLuint VAO;
  size_t IndicesCount;
  std::vector<Texture> Tex;

  std::vector<shared_model_data*> Children;
};

class ModelManager {
public:
  static ModelManager *getInstance();
  bool isModelLoaded(std::string path);
  void addModelData(std::string path, std::vector<shared_model_data*> data);
  inline std::vector<shared_model_data*> getLoadedModel(std::string path) {
    return mLoadedModels[path];
  }
private:
  static ModelManager* gModelManager;
  std::unordered_map<std::string, std::vector<shared_model_data*>> mLoadedModels;
  ModelManager();
};



#endif // !__MODEL_MANAGER_H__
