#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Object.h"
#include "Shader.h"
#include "Mesh.h"
#include "ModelManager.h"

class Model : public Object {
public:
  Model(char* path);
  //Model(Mesh* mesh);
  virtual void draw(Scene* context);

  void saveToShared(std::string path);
  void loadFromShared(std::string path);

  void setOpaque(bool is_opaque) {
      mIsOpaque = is_opaque;
  }
  void setCullFace(bool cull_face) {
      mIsCullFace = cull_face;
  }

private:
  std::vector<Texture> mTextureLoaded;
  std::string directory;

  bool mIsOpaque, mIsCullFace;

  void loadModel(std::string path);
  void processNode(aiNode* node, const aiScene* scene);
  Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

};

#endif // !__MODEL_H__
