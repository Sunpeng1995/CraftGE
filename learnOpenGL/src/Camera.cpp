#include "Camera.h"

Camera::Camera(float width, float height) : 
  Camera(width, height, glm::vec3(0.0f, 0.0f, 3.0f)) {

}

Camera::Camera(float width, float height, glm::vec3 initPos) :
  width(width), height(height),
  pos(initPos), front(glm::vec3(0.0f, 0.0f, -1.0f)), up(glm::vec3(0.0f, 1.0f, 0.0f)),
  pitch(0.0f), yaw(-90.0f), fov(45.0f), cameraSpeed(2.5f), sensitivity(0.05f)
{

}

void Camera::changePos(CameraDirection dir, float deltaTime) {
  float speed = deltaTime * cameraSpeed;
  switch (dir)
  {
  case forward:
    pos += speed * front;
    break;
  case back:
    pos -= speed * front;
    break;
  case left:
    pos -= speed *glm::normalize(glm::cross(front, up));
    break;
  case right:
    pos += speed *glm::normalize(glm::cross(front, up));
    break;
  default:
    break;
  }
}

void Camera::changeDirection(float xoffset, float yoffset) {
  yaw += xoffset * sensitivity;
  pitch += yoffset * sensitivity;

  if (pitch > 89.0f) {
    pitch = 89.0f;
  }
  if (pitch < -89.0f) {
    pitch = -89.0f;
  }

  glm::vec3 newFront;
  newFront.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
  newFront.y = sin(glm::radians(pitch));
  newFront.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
  front = glm::normalize(newFront);
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