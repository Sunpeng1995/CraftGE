#include "Scene.h"

Scene::Scene(int width, int height) : mWidth(width), mHeight(height) {
  mCamera = new Camera(width, height);
  mShader = new Shader("shader/simple.vert", "shader/simple.frag");
  mSkybox = nullptr;
  mLightingShader = nullptr;
  mDepthShader = nullptr;
  mShadowShader = nullptr;
  mPointDepthShader = nullptr;
  mFlashLight = nullptr;
  mDirLight = nullptr;

#ifdef DEBUG
  quadShader = new Shader("shader/debug_quad.vs", "shader/debug_quad_depth.fs");
  float quadVertices[] = {
    // positions        // texture Coords
    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
  };
  // setup plane VAO
  GLuint quadVBO;
  glGenVertexArrays(1, &quadVAO);
  glGenBuffers(1, &quadVBO);
  glBindVertexArray(quadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
#endif // DEBUG
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

#ifdef DEBUG
  quadShader->use();
  quadShader->setInt("depthMap", 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, mDepthMap);
  glBindVertexArray(quadVAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
#endif // DEBUG
#ifndef DEBUG


  drawLightings(mLightingShader);

  if (shadowEnable) {
    drawToDepthMap();
    mShadowShader->use();
    mShadowShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
    mShadowShader->setInt("shadowMap", 4);
    glActiveTexture(GL_TEXTURE0 + 4);
    glBindTexture(GL_TEXTURE_2D, mDepthMap);
    drawMeshes(mShadowShader);
  }
  if (pointShadowEnable) {
    drawToDepthMap();
    mPointShadowShader->use();
    mPointShadowShader->setFloat("far_plane", far_plane);
    mPointShadowShader->setInt("depthMap", 4);
    glActiveTexture(GL_TEXTURE0 + 4);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mDepthCubeMap);
    drawMeshes(mPointShadowShader);
  }
  if (!pointShadowEnable && !shadowEnable) {
    drawMeshes(mShader);
  }

  if (mSkybox) {
    mSkybox->draw(mCamera);
  }


#endif // !DEBUG

  if (lastTime == 0) {
    lastTime = glfwGetTime();
  }
  currentTime = glfwGetTime();
  fps++;
  if (currentTime - lastTime >= 1.0f) {
    std::cout << fps << "frame per second" << std::endl;
    fps = 0;
    lastTime = currentTime;
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

void Scene::enableShadow(Scene::ShadowType mode) {
  GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  switch (mode)
  {
  case Scene::parallel:
    if (mDirLight == nullptr) {
      return;
    }
    shadowEnable = true;
    pointShadowEnable = false;
    if (shadowInit) {
      return;
    }

    mDepthShader = new Shader("shader/simpleDepth.vert", "shader/simpleDepth.frag");
    mShadowShader = new Shader("shader/simpleShadow.vert", "shader/simpleShadow.frag");

    glGenFramebuffers(1, &mDepthMapFBO);

    glGenTextures(1, &mDepthMap);
    glBindTexture(GL_TEXTURE_2D, mDepthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, mDepthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    shadowInit = true;
    break;
  case Scene::point:
    if (mLights.size() == 0) {
      return;
    }
    pointShadowEnable = true;
    shadowEnable = false;
    if (pointShadowInit) {
      return;
    }

    mPointDepthShader = new Shader("shader/pointDepth.vert", "shader/pointDepth.geom", "shader/pointDepth.frag");
    mPointShadowShader = new Shader("shader/pointShadow.vert", "shader/pointShadow.frag");

    glGenFramebuffers(1, &mPointDepthMapFBO);

    glGenTextures(1, &mDepthCubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mDepthCubeMap);
    for (int i = 0; i < 6; i++) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, mPointDepthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mDepthCubeMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    pointShadowInit = true;

    break;
  default:
    break;
  }
}

void Scene::disableShadow() {
  shadowEnable = false;
  pointShadowEnable = false;
}

void Scene::drawToDepthMap() {
  if (shadowEnable) {
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, mDepthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    near_plane = 1.0f, far_plane = 7.5f;
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::vec3 lightpos = mDirLight->getPosition();
    glm::mat4 lightView = glm::lookAt(lightpos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    lightSpaceMatrix = lightProjection * lightView;

    mDepthShader->use();
    mDepthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

    drawMeshes(mDepthShader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, mCamera->scrWidth(), mCamera->scrHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
  if (pointShadowEnable) {
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, mPointDepthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    Light* point = mLights[0];
    glm::vec3 lightPos = point->getPosition();

    near_plane = 1.0f, far_plane = 25.0f;
    GLfloat aspect = (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT;
    glm::mat4 lightProjection = glm::perspective(glm::radians(90.0f), aspect, near_plane, far_plane);

    mPointLightSpaceMatrix.push_back(lightProjection *
      glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    mPointLightSpaceMatrix.push_back(lightProjection *
      glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    mPointLightSpaceMatrix.push_back(lightProjection *
      glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    mPointLightSpaceMatrix.push_back(lightProjection *
      glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    mPointLightSpaceMatrix.push_back(lightProjection *
      glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    mPointLightSpaceMatrix.push_back(lightProjection *
      glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

    mPointDepthShader->use();
    mPointDepthShader->setFloat("far_plane", far_plane);
    for (int i = 0; i < 6; i++) {
      mPointDepthShader->setMat4("pointLightSpaceMatrices[" + std::to_string(i) + "]", mPointLightSpaceMatrix[i]);
    }
    mPointDepthShader->setVec3("lightPos", lightPos);

    drawMeshes(mPointDepthShader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, mCamera->scrWidth(), mCamera->scrHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
}