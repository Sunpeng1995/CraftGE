#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Object.h"
#include "Shader.h"
#include "Mesh.h"
#include "Shadow.h"

class Light :public Object {
public:
  Light();
  Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
  virtual void passToShader(Shader* shader) = 0;
  virtual void draw(Shader* shader) = 0;
  void setLightMesh(Mesh* mesh);

  void setAmbient(glm::vec3 ambient);
  void setAmbient(float x, float y, float z);
  void setDiffuse(glm::vec3 diffuse);
  void setDiffuse(float x, float y, float z);
  void setSpecular(glm::vec3 specular);
  void setSpecular(float x, float y, float z);

protected:
  const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
  glm::vec3 mAmbient, mDiffuse, mSpecular;
  Mesh* mLightMesh;
};

class DirLight : public Light {
public:
  DirLight(glm::vec3 direction);
  DirLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
  virtual void passToShader(Shader* shader);
  virtual void draw(Shader* shader);

  void setDirection(glm::vec3 direction);
  void setDirection(float x, float y, float z);

private:
  glm::vec3 mDirection;
};

class PointLight : public Light {
public:
  PointLight(int id);
  PointLight(int id, glm::vec3 position);
  PointLight(int id, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
    float constant, float linear, float quadratic);
  virtual void passToShader(Shader* shader);
  virtual void draw(Shader* shader);

  void setConstant(float constant);
  void setLinear(float linear);
  void setQuadratic(float quadratic);

private:
  int mID;
  std::string mName;
  float mConstant, mLinear, mQuadratic;
};

class SpotLight : public Light {
public:
  SpotLight(int id);
  SpotLight(int id, glm::vec3 position, glm::vec3 direction, float cutoff, float outercutoff);
  SpotLight(int id, glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
    float cutoff, float outercutoff);
  virtual void passToShader(Shader* shader);
  virtual void draw(Shader* shader);
  void updateFlashLight(glm::vec3 position, glm::vec3 direction);

  void setDirection(glm::vec3 dir);
  void setDirection(float x, float y, float z);
  void setCutOff(float cutoff);
  void setOuterCutOff(float outercutoff);

private:
  int mID;
  std::string mName;
  glm::vec3 mDirection;
  float mCutOff, mOuterCutOff;
};

#endif // !__LIGHT_H__
