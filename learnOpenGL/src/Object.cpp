#include "Object.h"

using namespace glm;

Object::Object() : Object(vec3(0, 0, 0))
{
}

Object::Object(vec3 position) : Object(position, vec3(0, 0, 0), vec3(1, 1, 1))
{
}

Object::Object(vec3 position, vec3 rotation) : 
  Object(position, rotation, vec3(1, 1, 1))
{
}

Object::Object(vec3 position, vec3 rotation, vec3 scale) :
  mPosition(position), mLocalPosition(position),
  mRotation(rotation), mScale(scale)
{
  mForward = vec3(0, 0, -1.0f);
  mUp = vec3(0, 1.0f, 0);
  mRight = normalize(cross(mForward, mUp));
}

void Object::addChild(Object* child) {
}

mat4 Object::calcRotationMatrixFromForward(vec3 target) {
  vec3 v = cross(mForward, target);
  float angle = glm::acos(dot(mForward, target) / length(mForward) * length(target));
  return rotate(mat4(), angle, v);
}

void Object::setForward(vec3 target) {
  // TODO: correct?
  mat3 rotate(calcRotationMatrixFromForward(target));
  mForward = rotate * mForward;
  mUp = rotate * mUp;
  mRight = cross(mForward, mUp);
}