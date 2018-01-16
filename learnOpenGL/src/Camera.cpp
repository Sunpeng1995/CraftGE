#include "Camera.h"

using namespace glm;

Camera::Camera(int width, int height) : 
  Camera(width, height, vec3(0.0f, 0.0f, 3.0f)) {

}

Camera::Camera(int width, int height, vec3 initPos) :
  Object(initPos, vec3(0, -90.0f, 0)), 
  screenWidth(width), screenHeight(height),
  fov(45.0f), cameraSpeed(2.5f), sensitivity(0.05f)
{
  canChangeDirection = false;
}

void Camera::changePos(CameraDirection dir, float deltaTime) {
  float speed = deltaTime * cameraSpeed;
  switch (dir)
  {
  case forward:
    mPosition += speed * mForward;
    break;
  case back:
    mPosition -= speed * mForward;
    break;
  case left:
    mPosition -= speed * mRight;
    break;
  case right:
    mPosition += speed * mRight;
    break;
  default:
    break;
  }
}

void Camera::changeDirection(float xoffset, float yoffset) {
  if (!canChangeDirection) {
    return;
  }
  mRotation.y += xoffset * sensitivity;
  mRotation.x += yoffset * sensitivity;

  if (mRotation.x > 89.0f) {
    mRotation.x = 89.0f;
  }
  if (mRotation.x < -89.0f) {
    mRotation.x = -89.0f;
  }

  vec3 newFront;
  newFront.x = cos(radians(mRotation.x)) * cos(radians(mRotation.y));
  newFront.y = sin(radians(mRotation.x));
  newFront.z = cos(radians(mRotation.x)) * sin(radians(mRotation.y));
  mForward = normalize(newFront);
  mRight = normalize(cross(mForward, mUp));
}

void Camera::changeZoom(float yoffset) {
  fov -= yoffset;
  if (fov <= 1.0f) {
    fov = 1.0f;
  }
  if (fov >= 60.0f) {
    fov = 60.0f;
  }
}