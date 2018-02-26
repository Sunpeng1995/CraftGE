#include "Input.h"

InputSystem::InputSystem(GLFWwindow* window) : mWindow(window) {

}

InputSystem* InputSystem::getInstance() {
  return gInputSystem;
}