#ifndef __INPUT_SYSTEM_H__
#define __INPUT_SYSTEM_H__

#include <GLFW\glfw3.h>

class InputSystem {
public:
  InputSystem(GLFWwindow *window);
  static InputSystem* getInstance();

private:
  GLFWwindow *mWindow;

  InputSystem() {}
};

static InputSystem *gInputSystem;

#endif // !__INPUT_SYSTEM_H__
