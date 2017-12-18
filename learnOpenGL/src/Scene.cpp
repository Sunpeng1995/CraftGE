#include "Scene.h"

Scene::Scene(int width, int height) : mWidth(width), mHeight(height) {
  mCamera = new Camera(width, height);
  mShader = new Shader("shader/simple.vert", "shader/simple.frag");
}

Scene::~Scene() {
  delete mCamera;
  delete mShader;

  for (int i = 0; i < mMeshes.size(); i++) {
    delete mMeshes[i];
  }
}

void Scene::addMesh(Mesh* mesh) {
  mMeshes.push_back(mesh);
}

void Scene::draw() {
  mShader->use();
  mShader->setMat4("view", mCamera->view());
  mShader->setMat4("proj", mCamera->projection());

  drawMeshes(mShader);
}

void Scene::drawMeshes(Shader* shader) {
  for (auto m : mMeshes) {
    m->draw(shader);
  }
}