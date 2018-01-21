#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Object.h"
#include "Shader.h"
#include "Mesh.h"

class Model : public Object {
public:
  Model(char* path);
  //Model(Mesh* mesh);
  virtual void draw(Shader* shader);

  //inline void setScale(float scale) {
  //  //for (auto m : mMeshes) {
  //  //  m.setScale(mScale);
  //  //}
  //  for (int i = 0; i < mMeshes.size(); i++) {
  //    mMeshes[i].setScale(mScale);
  //  }
  //}

  //inline void setPosition(glm::vec3 pos) {
  //  mPosition = pos;
  //  for (int i = 0; i < mMeshes.size(); i++) {
  //    mMeshes[i].setPos(mPosition);
  //  }
  //}

private:
  std::vector<Texture> mTextureLoaded;
  std::string directory;

  void loadModel(std::string path);
  void processNode(aiNode* node, const aiScene* scene);
  Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

};

#endif // !__MODEL_H__
