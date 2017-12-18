#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Mesh.h"

class Light {
public:
  Light();
  Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
  virtual void passToShader(Shader* shader) = 0;
  virtual void draw(Shader* shader) = 0;
  void setMesh(Mesh* mesh);

protected:
  glm::vec3 mAmbient, mDiffuse, mSpecular;
  Mesh* mMesh;
};

class DirLight : public Light {
public:
  DirLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
  virtual void passToShader(Shader* shader);
  virtual void draw(Shader* shader);

private:
  glm::vec3 mDirection;
};

class PointLight : public Light {
public:
  PointLight(int id, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
    float constant, float linear, float quadratic);
  virtual void passToShader(Shader* shader);
  virtual void draw(Shader* shader);
private:
  int mID;
  std::string mName;
  glm::vec3 mPosition;
  float mConstant, mLinear, mQuadratic;
};

class SpotLight : public Light {
public:
  SpotLight(int id, glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
    float cutoff, float outercutoff);
  virtual void passToShader(Shader* shader);
  virtual void draw(Shader* shader);
  void updateFlashLight(glm::vec3 position, glm::vec3 direction);
private:
  int mID;
  std::string mName;
  glm::vec3 mPosition, mDirection;
  float mCutOff, mOuterCutOff;
};

#endif // !__LIGHT_H__
