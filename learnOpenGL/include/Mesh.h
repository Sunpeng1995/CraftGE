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
  glm::vec3 mPos;
  GLuint mVAO;

  float mRotateAngle;
  glm::vec3 mRotateAxis;
};

#endif // !__MESH_H__
