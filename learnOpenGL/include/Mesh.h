#ifndef __MESH_H__
#define __MESH_H__

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Texture.h"

class Mesh {
public:
  virtual void draw(Shader* shader) = 0;
  void addTexture(Texture* tex);
  Texture* getTexture(int idx);

  void setScale(float scale);
  void setPos(glm::vec3 pos);
  inline glm::vec3 getPos() {
    return mPos;
  }
protected:
  glm::vec3 mPos;
  float mScale;
private:
  std::vector<Texture*> mTextures;
};

class Cube : public Mesh {
public:
  Cube();
  Cube(glm::vec3 pos);
  virtual void draw(Shader* shader);

  void setRotate(float angle, glm::vec3 axis);

private:
  GLuint mVAO;
  float mRotateAngle;
  glm::vec3 mRotateAxis;
};

class NormalledCube : public Mesh {
public:
  NormalledCube();
  NormalledCube(glm::vec3 pos);

  virtual void draw(Shader* shader);
  void setRotate(float angle, glm::vec3 axis);
private:
  GLuint mVAO;
  float mRotateAngle;
  glm::vec3 mRotateAxis;
};

#endif // !__MESH_H__
