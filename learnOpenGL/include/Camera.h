#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Object.h"
#include "Shader.h"

enum CameraDirection {
  forward,
  back,
  left,
  right
};

enum CameraType {
  ortho,
  perspective
};

class Camera :public Object {
public:
  Camera(int width, int height);
  Camera(int width, int height, glm::vec3 initPos);
  virtual void draw(Shader* shader) 
  {
  }

  inline void bindShader(const Shader &shader) {
    shaders.push_back(&shader);
  }

  inline glm::mat4 view() {
    return glm::lookAt(mPosition, mPosition + mForward, mUp);
  }

  inline glm::mat4 projection() {
    return glm::perspective(glm::radians(fov), (float)screenWidth / screenHeight, 0.1f, 100.0f);
  }

  inline void updateScreen(float width, float height) {
    this->screenWidth = width;
    this->screenHeight = height;
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

  inline glm::vec3 getPos() {
    return mPosition;
  }

  inline glm::vec3 getFront() {
    return mForward;
  }

  inline int scrWidth() {
    return screenWidth;
  }
  inline int scrHeight() {
    return screenHeight;
  }

private:
  int screenWidth, screenHeight;
  float fov;
  float cameraSpeed, sensitivity;
  
  bool canChangeDirection;

  std::vector<const Shader*> shaders;

};


#endif // !__CAMERA_H__