#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Light.h"

class Scene {
public:
  Scene(int width, int height);
  virtual ~Scene();

  inline void updateScreen(int width, int height) {
    mWidth = width;
    mHeight = height;
  }

  void addMesh(Mesh* mesh);
  void setLightingObject(Mesh* mesh);
  void setObjectShader(Shader* shader);
  void setLightingShader(Shader* shader);
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

private:
  int mWidth, mHeight;
  Camera* mCamera;
  Shader* mShader, *mLightingShader;

  std::vector<Mesh*> mMeshes;
  //Mesh* mLightingObj;
  std::vector<Light*> mLights;
  DirLight* mDirLight;
  SpotLight* mFlashLight;

  void drawMeshes(Shader* shader);
  void drawLightings(Shader* shader);
};

#endif // !__SCENE_H__
