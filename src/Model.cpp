#include "Model.h"

#include "Scene.h"

using namespace glm;

Model::Model(char* path) : Object(vec3(0.0f)), mIsOpaque(true), mIsCullFace(true) {
  if (ModelManager::getInstance()->isModelLoaded(path)) {
    loadFromShared(path);
  }
  else {
    loadModel(path);
    saveToShared(path);
  }
}

void Model::draw(Scene* context) {
    context->passContextToShader(getShader());
    if (!mIsOpaque) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    if (!mIsCullFace) {
        glDisable(GL_CULL_FACE);
    }

  for (auto i : mChildren) {
    i->draw(context);
  }

    if (!mIsOpaque) {
        glBlendFunc(GL_ONE, GL_ZERO);
    }
    if (!mIsCullFace) {
        glEnable(GL_CULL_FACE);
    }
}

void Model::saveToShared(std::string path) {
  std::vector<shared_model_data*> data;
  for (auto i : mChildren) {
    data.push_back(i->packSharedData());
  }
  ModelManager::getInstance()->addModelData(path, data);
}

void Model::loadFromShared(std::string path) {
  auto data = ModelManager::getInstance()->getLoadedModel(path);
  for (auto i : data) {
    auto m = new Mesh(i);
    m->setParent(this);
    mChildren.push_back(m);
  }
}

void Model::loadModel(std::string path) {
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace/* | aiProcess_FlipUVs*/);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    return;
  }

  directory = path.substr(0, path.find_last_of('/'));

  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
  for (int i = 0; i < node->mNumMeshes; i++) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    Mesh* child = processMesh(mesh, scene);
    child->setParent(this);
    mChildren.push_back(child);
  }

  for (int i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene);
  }
}

Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  for (int i = 0; i < mesh->mNumVertices; i++) {
    Vertex v;
    glm::vec3 v3;
    v3.x = mesh->mVertices[i].x;
    v3.y = mesh->mVertices[i].y;
    v3.z = mesh->mVertices[i].z;
    v.Position = v3;

    v3.x = mesh->mNormals[i].x;
    v3.y = mesh->mNormals[i].y;
    v3.z = mesh->mNormals[i].z;
    v.Normal = v3;

    v3.x = mesh->mTangents[i].x;
    v3.y = mesh->mTangents[i].y;
    v3.z = mesh->mTangents[i].z;
    v.Tangent = v3;

    if (mesh->mTextureCoords[0]) {
      glm::vec2 v2;
      v2.x = mesh->mTextureCoords[0][i].x;
      v2.y = mesh->mTextureCoords[0][i].y;
      v.TexCoords = v2;
    }
    else {
      v.TexCoords = glm::vec2(0.0f, 0.0f);
    }

    vertices.push_back(v);
  }

  for (int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  if (mesh->mMaterialIndex >= 0) {
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
  }

  return new Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
  std::vector<Texture> textures;

  for (int i = 0; i < mat->GetTextureCount(type); i++) {
    aiString str;
    mat->GetTexture(type, i, &str);
    bool skip = false;
    std::string complete_path = directory + "/" + std::string(str.C_Str());
    for (int i = 0; i < mTextureLoaded.size(); i++) {
      if (mTextureLoaded[i].getPath() == complete_path) {
        textures.push_back(mTextureLoaded[i]);
        skip = true;
        break;
      }
    }
    if (!skip) {
      Texture t(complete_path, typeName);
      textures.push_back(t);
      mTextureLoaded.push_back(t);
    }
  }

  return textures;
}
