#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

enum CameraDirection {
  forward,
  back,
  left,
  right
};

class Camera {
public:
  Camera(int width, int height);
  Camera(int width, int height, glm::vec3 initPos);

  inline void bindShader(const Shader &shader) {
    shaders.push_back(&shader);
  }

  inline glm::mat4 view() {
    return glm::lookAt(pos, pos + front, up);
  }

  inline glm::mat4 projection() {
    return glm::perspective(glm::radians(fov), (float)width / height, 0.1f, 100.0f);
  }

  inline void updateScreen(float width, float height) {
    this->width = width;
    this->height = height;
  }

  void changePos(CameraDirection dir, float deltaTime);
  void changeDirection(float xoffset, float yoffset);
  void changeZoom(float yoffset);
  void enableChangeDirection() {
    canChangeDirection = true;
  }
  void disableChangeDirection() {
    canChangeDirection = false;
  }

private:
  int width, height;
  glm::vec3 pos, front, up;
  float pitch, yaw, fov;
  float cameraSpeed, sensitivity;
  
  bool canChangeDirection;

  std::vector<const Shader*> shaders;

};


#endif // !__CAMERA_H__