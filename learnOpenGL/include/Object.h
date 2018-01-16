#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Object {
public:
  void addChild(Object* child);

  inline glm::vec3 getPosition() {
    return mPosition;
  }
  inline void setPosition(glm::vec3 pos) {
    mPosition = pos;
  }
  inline void setPosition(float x, float y, float z) {
    setPosition(glm::vec3(x, y, z));
  }
  inline glm::vec3 getLocalPosition() {
    return mLocalPosition;
  }

  glm::mat4 calcRotationMatrixFromForward(glm::vec3 target);

protected:
  // Object position in the world space
  glm::vec3 mPosition;
  // Object position in the parent's space
  glm::vec3 mLocalPosition;
  // Euler angle: x-pitch, y-yaw, z-roll
  glm::vec3 mRotation;
  glm::vec3 mScale;

  glm::vec3 mForward, mUp, mRight;

  unsigned int mChildCount;

  std::vector<Object*> mChildren;
  Object* mParent;
  
  Object(glm::vec3 position);
  Object(glm::vec3 position, glm::vec3 rotation);
  Object(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

  void setForward(glm::vec3 target);
private:
  Object();
};

#endif // !__OBJECT_H__
