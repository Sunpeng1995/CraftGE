#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Light.h"
#include "Model.h"
#include "Skybox.h"

class Scene {
public:
  Scene(int width, int height);
  virtual ~Scene();

  inline void updateScreen(int width, int height) {
    mWidth = width;
    mHeight = height;
  }

  void addMesh(Mesh* mesh);
  void addModel(Model* model);
  void setLightingObject(Mesh* mesh);
  void setObjectShader(Shader* shader);
  void setLightingShader(Shader* shader);
  void setSkybox(Skybox* skybox);
  void draw();

  void setDirLight(DirLight* light);
  void setFlashLight(SpotLight* light);
  void addOtherLight(Light* light);

  inline Camera& getCamera() {
    return *mCamera;
  }

  inline void setLineModeOn() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  inline void setFillModeOn() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  inline void switchFlashLight(bool enable) {
    enableFlashLight = enable;
  }

private:
  bool enableFlashLight = true;
  int mWidth, mHeight;
  Camera* mCamera;
  Shader* mShader, *mLightingShader;
  Skybox* mSkybox;

  std::vector<Mesh*> mMeshes;
  std::vector<Model*> mModels;
  //Mesh* mLightingObj;
  std::vector<Light*> mLights;
  DirLight* mDirLight;
  SpotLight* mFlashLight;

  void drawMeshes(Shader* shader);
  void drawLightings(Shader* shader);
};

#endif // !__SCENE_H__
