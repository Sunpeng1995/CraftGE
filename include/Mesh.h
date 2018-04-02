#ifndef __MESH_H__
#define __MESH_H__

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Object.h"
#include "Shader.h"
#include "Texture.h"
#include "ModelManager.h"

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
  glm::vec3 Tangent;
};

class Mesh :public Object {
public:
  Mesh();
  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, int render_layer = 0);
  Mesh(shared_model_data* data);
  virtual void draw(Shader* shader);
  void addTexture(Texture tex);

  void setPos(glm::vec3 pos);

  inline int getRenderLayer() {
      return mRenderLayer;
  }

  inline glm::vec3 getPos() {
    return mPosition;
  }

  bool operator<(Mesh &that) {
      return this->mRenderLayer < that.getRenderLayer();
  }

  virtual shared_model_data* packSharedData();

protected:
  std::vector<Vertex> mVertices;
  std::vector<unsigned int> mIndices;
  std::vector<Texture> mTextures;
private:
  unsigned int mVAO, mVBO, mEBO;
  size_t mIndicesCount;
  int mRenderLayer;
};

class Cube : public Mesh {
public:
  Cube();
  Cube(glm::vec3 pos);
  virtual void draw(Shader* shader);

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
  void setReverse(bool r) {
    mReverse = r;
  }
private:
  bool mReverse = false;
  GLuint mVAO;
  float mRotateAngle;
  glm::vec3 mRotateAxis;
};

class Plane : public Mesh {
public:
  Plane();
  Plane(glm::vec3 pos);

  virtual void draw(Shader* shader);
private:
  GLuint mVAO;
};

#endif // !__MESH_H__
