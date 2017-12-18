#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"

class Scene {
public:
  Scene(int width, int height);
  virtual ~Scene();

  inline void updateScreen(int width, int height) {
    mWidth = width;
    mHeight = height;
  }

  void addMesh(Mesh* mesh);
  void draw();

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
  Shader* mShader;

  std::vector<Mesh*> mMeshes;
  void drawMeshes(Shader* shader);
};

#endif // !__SCENE_H__
