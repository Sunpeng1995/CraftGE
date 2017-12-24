#include "Scene.h"

Scene::Scene(int width, int height) : mWidth(width), mHeight(height) {
  mCamera = new Camera(width, height);
  mShader = new Shader("shader/simple.vert", "shader/simple.frag");
  mSkybox = nullptr;
  mLightingShader = nullptr;
  mFlashLight = nullptr;
  mDirLight = nullptr;
}

Scene::~Scene() {
  delete mCamera;
  delete mShader;

  for (int i = 0; i < mMeshes.size(); i++) {
    delete mMeshes[i];
  }
  for (int i = 0; i < mLights.size(); i++) {
    delete mLights[i];
  }
}

void Scene::addMesh(Mesh* mesh) {
  mMeshes.push_back(mesh);
}

void Scene::addModel(Model* model) {
  mModels.push_back(model);
}

void Scene::setLightingObject(Mesh* mesh) {
  //mLightingObj = mesh;
}

void Scene::setObjectShader(Shader* shader) {
  Shader* oldShader = mShader;
  mShader = shader;
  if (oldShader) delete oldShader;
}

void Scene::setLightingShader(Shader* shader) {
  Shader* oldShader = mLightingShader;
  mLightingShader = shader;
  if (oldShader) delete oldShader;
}

void Scene::setDirLight(DirLight* light) {
  DirLight* old = mDirLight;
  mDirLight = light;
  if (old) delete old;
}

void Scene::setFlashLight(SpotLight* light) {
  SpotLight* old = mFlashLight;
  mFlashLight = light;
  if (old) delete old;
}

void Scene::setSkybox(Skybox* skybox) {
  Skybox* old = mSkybox;
  mSkybox = skybox;
  if (old) delete old;
}

void Scene::addOtherLight(Light* light) {
  mLights.push_back(light);
}

void Scene::draw() {
  drawLightings(mLightingShader);
  drawMeshes(mShader);
  if (mSkybox) {
    mSkybox->draw(mCamera);
  }
}

void Scene::drawMeshes(Shader* shader) {
  shader->use();
  shader->setMat4("view", mCamera->view());
  shader->setMat4("projection", mCamera->projection());

  //if (mLightingObj) {
  //  glm::vec3 lightPos = mLightingObj->getPos();
  //  //shader->setVec3("lightPos", lightPos);
  //  //shader->setVec3("light.direction", -0.2f, -1.0f, -0.3f);
  //  shader->setVec3("light.position", lightPos);
  //  shader->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
  //  shader->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
  //  shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
  //  shader->setVec3("light.position", mCamera->getPos());
  //  shader->setVec3("light.direction", mCamera->getFront());
  //  shader->setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
  //  shader->setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
  //  //shader->setFloat("light.constant", 1.0f);
  //  //shader->setFloat("light.linear", 0.09f);
  //  //shader->setFloat("light.quadratic", 0.032f);
  //}

  shader->setVec3("viewPos", mCamera->getPos());
  shader->setFloat("material.shininess", 32.0f);

  if (mDirLight) {
    mDirLight->passToShader(shader);
  }

  for (auto l : mLights) {
    l->passToShader(shader);
  }

  if (mFlashLight && enableFlashLight) {
    mFlashLight->updateFlashLight(mCamera->getPos(), mCamera->getFront());
    mFlashLight->passToShader(shader);
  }

  for (auto m : mMeshes) {
    m->draw(shader);
  }

  for (auto m : mModels) {
    m->draw(shader);
  }
}

void Scene::drawLightings(Shader* shader) {
  if (!shader) return;
  shader->use();
  shader->setMat4("view", mCamera->view());
  shader->setMat4("projection", mCamera->projection());

  //mLightingObj->draw(shader);
  for (auto l : mLights) {
    l->draw(shader);
  }
}