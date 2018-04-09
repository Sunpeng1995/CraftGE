#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include <vector>
#include "Scene.h"
#include "ParticleSystem.h"
#include "Mesh.h"

class SceneManager {
public:
  static SceneManager *getInstance();

  ~SceneManager();

  void addScene(Scene* scene);
  Scene* findScene(std::string scene_name);
  void setCurrentScene(std::string scene_name);
  void setCurrentScene(Scene* scene);
  Scene* getCurrentScene();
  void setScreenWidth(unsigned int width);
  void setScreenHeight(unsigned int height);
  void setScreenSize(unsigned int width, unsigned int height);

  ///////////////////
  // Example Scenes
  void createBasicScene();
  void createLightingScene();
  void createModelScene();
  void createNormalScene();
  void createShadowScene();
  void createPointShadowScene();
  void createDeferredShadingScene();
  void createFoggedScene();
  void createParticlesScene();
  void createParticlesAnimateScene();
  void createWaterScene();

  void createAllExampleScenes();

private:
  static SceneManager* gSceneManager;
  std::vector<Scene*> mLoadedScenes;
  Scene* mCurrentScene;
  unsigned int mScreenWidth, mScreenHeight;

  SceneManager();
  void updateScreenSize();

  ///////////////////
  // Example Scenes Data
  glm::vec3 cubePositions[10] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
  };

  glm::vec3 objectPositions[9] = {
    glm::vec3(-3.0, -2.0, -3.0),
    glm::vec3(0.0, -2.0, -3.0),
    glm::vec3(3.0, -2.0, -3.0),
    glm::vec3(-3.0, -2.0, 0.0),
    glm::vec3(0.0, -2.0, 0.0),
    glm::vec3(3.0, -2.0, 0.0),
    glm::vec3(-3.0, -2.0, 3.0),
    glm::vec3(0.0, -2.0, 3.0),
    glm::vec3(3.0, -2.0, 3.0)
  };

};

#endif // !__SCENE_MANAGER_H__
