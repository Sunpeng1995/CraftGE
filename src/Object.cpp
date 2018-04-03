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
  mPosition(position), mWorldPosition(position),
  mRotation(rotation), mScale(scale), mParent(nullptr), mShader(nullptr)
{
  mForward = vec3(0, 0, -1.0f);
  mUp = vec3(0, 1.0f, 0);
  mRight = normalize(cross(mForward, mUp));

  mRotationMatrix = glm::mat4(1.0f);
  mModelMatrix = glm::mat4(1.0f);
  mParentModelMatrix = glm::mat4(1.0f);

  calcModelMatrix();
}

Object::~Object() {

}

void Object::addChild(Object* child) {
  mChildren.push_back(child);
  child->mParent = this;
  child->updateFromParent();
}

mat4 Object::calcRotationMatrixFromForward(vec3 target) {
  vec3 v = cross(mForward, target);
  float angle = glm::acos(dot(mForward, target) / (length(mForward) * length(target)));
  return rotate(mat4(1.0f), angle, v);
}

void Object::calcModelMatrix() {
  mRotationMatrix = eulerAngleXYZ(mRotation.x, mRotation.y, mRotation.z);

  mModelMatrix = translate(mat4(1.0f), mPosition);
  mModelMatrix = mModelMatrix * mRotationMatrix;
  mModelMatrix = glm::scale(mModelMatrix, mScale);

  applyToChildren();
}

void Object::setForward(vec3 target) {
  // TODO: correct?
  mat3 rotate(calcRotationMatrixFromForward(target));
  mForward = rotate * mForward;
  mUp = rotate * mUp;
  mRight = cross(mForward, mUp);
}

void Object::applyToChildren() {
  for (auto i : mChildren) {
    i->updateFromParent();
    i->applyToChildren();
  }
}

void Object::updateFromParent() {
  if (mParent) {
    mParentModelMatrix = mParent->getParentModelMatrix() * mParent->getModelMatrix();
  }
}

void Object::update(float delta_time) {
  if (updateFunc) {
    updateFunc(this);
  }
  for (auto i : mChildren) {
    i->update(delta_time);
  }
}

Shader* Object::getShader() {
    if (mShader == nullptr) {
        if (mParent) {
            return mParent->getShader();
        }
        else {
            return nullptr;
        }
    }
    return mShader;
}