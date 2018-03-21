#ifndef __SCENE_H__
#define __SCENE_H__

//#define DEBUG

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
  enum ShadowType {
    parallel,
    point
  };

  enum ShadingType {
    forward,
    deferred
  };


  Scene(int width, int height, std::string scene_name, ShadingType st = forward);
  virtual ~Scene();

  inline void updateScreen(int width, int height) {
    mWidth = width;
    mHeight = height;
  }

  void addObject(Object* object);
  void addMesh(Mesh* mesh);
  void addModel(Model* model);
  void addOtherTextures(Texture tex);
  void setLightingObject(Mesh* mesh);
  void setObjectShader(Shader* shader);
  void setLightingShader(Shader* shader);
  void setSkybox(Skybox* skybox);
  void draw();
  void update();

  void setDirLight(DirLight* light);
  void setFlashLight(SpotLight* light);
  void addOtherLight(Light* light);

  void enableShadow(ShadowType mode);
  void disableShadow();
  void drawToDepthMap();

  inline Camera& getCamera() {
    return *mCamera;
  }

  inline Camera* getCameraPointer() {
      return mCamera;
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

  inline std::string getName() {
    return mName;
  }

private:
  ShadingType mShadingType;
  std::string mName;

  bool enableFlashLight = true;
  int mWidth, mHeight;
  Camera* mCamera;
  Shader* mShader, *mLightingShader;
  Skybox* mSkybox;

  // Shadow
  const int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
  GLuint mDepthMapFBO;
  GLuint mPointDepthMapFBO;

  GLuint mDepthMap;
  Shader *mDepthShader, *mShadowShader;
  bool shadowEnable = false, shadowInit = false;
  glm::mat4 lightSpaceMatrix;
  //// Point Shadow
  bool pointShadowEnable = false, pointShadowInit = false;
  GLfloat near_plane, far_plane;
  GLuint mDepthCubeMap;
  Shader *mPointDepthShader, *mPointShadowShader;
  std::vector<glm::mat4> mPointLightSpaceMatrix;

  std::vector<Object*> mMeshes;
  std::vector<Model*> mModels;
  //Mesh* mLightingObj;
  std::vector<Light*> mLights;
  std::vector<Texture> mOtherTextures;
  DirLight* mDirLight;
  SpotLight* mFlashLight;


  // Deferred Shading
  GLuint gBuffer;
  GLuint gPosition, gNormal, gAlbedoSpec;
  Shader* gBufferShader, *gLightingShader;
  GLuint gQuadVAO;
  void setupGBuffer();
  void drawGBufferInQuad();


  int fps = 0;
  double lastTime = 0, currentTime;
  double delta_time, lastRecordTime;

  //Debug
#ifdef DEBUG
  GLuint quadVAO = 0;
  Shader* quadShader;
#endif // DEBUG

  void drawMeshes(Shader* shader);
  void drawLightings(Shader* shader);
};

#endif // !__SCENE_H__
