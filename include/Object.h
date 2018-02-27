#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <vector>
#include <functional>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "ModelManager.h"

class Shader;

class Object {
public:
  virtual ~Object();

  void addChild(Object* child);

  inline glm::vec3 getPosition() {
    return mPosition;
  }
  inline void setPosition(glm::vec3 pos) {
    mPosition = pos;
    calcModelMatrix();
  }
  inline void setPosition(float x, float y, float z) {
    setPosition(glm::vec3(x, y, z));
  }
  inline glm::vec3 getWorldPosition() {
    return mWorldPosition;
  }

  inline glm::vec3 getRotation() {
    return mRotation;
  }

  inline void setRotation(glm::vec3 rotate) {
    // TODO: limit angle range
    mRotation = rotate;
    calcModelMatrix();
  }

  inline void setRotation(float pitch, float yaw, float roll) {
    setRotation(glm::vec3(pitch, yaw, roll));
  }

  inline glm::vec3 getScale() {
    return mScale;
  }
  inline void setScale(glm::vec3 scale) {
    mScale = scale;
    calcModelMatrix();
  }
  inline void setScale(float scale) {
    setScale(glm::vec3(scale));
  }
  inline void setScale(float x, float y, float z) {
    setScale(glm::vec3(x, y, z));
  }

  inline glm::mat4 getModelMatrix() {
    return mModelMatrix;
  }

  inline glm::mat4 getParentModelMatrix() {
    return mParentModelMatrix;
  }

  inline void updateRegister(std::function<void(Object*)> func) {
    updateFunc = func;
  }

  inline void setParent(Object* parent) {
    mParent = parent;
  }

  glm::mat4 calcRotationMatrixFromForward(glm::vec3 target);

  virtual void draw(Shader* shader) = 0;
  virtual void update();
  virtual shared_model_data* packSharedData() { return nullptr; }

protected:
  // Object position in the parent's space
  glm::vec3 mPosition;
  // Object position in the world space
  glm::vec3 mWorldPosition;
  // Euler angle: x-pitch, y-yaw, z-roll
  glm::vec3 mRotation;
  glm::vec3 mScale;

  glm::vec3 mForward, mUp, mRight;
  glm::mat4 mRotationMatrix;

  glm::mat4 mModelMatrix;
  glm::mat4 mParentModelMatrix;

  std::vector<Object*> mChildren;
  Object* mParent;
  
  Object(glm::vec3 position);
  Object(glm::vec3 position, glm::vec3 rotation);
  Object(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

  void setForward(glm::vec3 target);
private:
  Object();
  void calcModelMatrix();
  void applyToChildren();
  void updateFromParent();

  std::function<void(Object*)> updateFunc;
};

#endif // !__OBJECT_H__
