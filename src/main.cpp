#include <iostream>
#include <fstream>
#include <string>
#include <streambuf>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "Shader.h"
#include "Camera.h"
#include "Scene.h"
#include "Model.h"
#include "Skybox.h"
#include "Input.h"
#include "SceneManager.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

Scene* createBasicScene();
Scene* createLightingScene();
Scene* createModelScene();
Scene* createNormalScene();
Scene* createShadowScene();
Scene* createPointShadowScene();
Scene* createDeferredShadingScene();
Scene* createForwardShadingScene();

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float sWidth = 1280.0f;
float sHeight = 720.0f;

bool firstMouse = true;
float lastX, lastY;

bool isNormal = true;

int main() {
  // init glfw
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwWindowHint(GLFW_SAMPLES, 4);

  // create a glfw window
  GLFWwindow* window = glfwCreateWindow(sWidth, sHeight, "CraftGE", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create glfw window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  gInputSystem = new InputSystem(window);

  // init glad before calling any OpenGL function
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  glViewport(0, 0, sWidth, sHeight);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  SceneManager::getInstance()->setScreenSize(sWidth, sHeight);
  SceneManager::getInstance()->createAllExampleScenes();

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    SceneManager::getInstance()->getCurrentScene()->update();
    SceneManager::getInstance()->getCurrentScene()->draw();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
  SceneManager::getInstance()->setScreenSize(width, height);
}

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  auto scene = SceneManager::getInstance()->getCurrentScene();

  float CurrentTime = glfwGetTime();
  deltaTime = CurrentTime - lastFrame;
  lastFrame = CurrentTime;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    scene->getCamera().changePos(CameraDirection::forward, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    scene->getCamera().changePos(CameraDirection::back, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    scene->getCamera().changePos(CameraDirection::left, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    scene->getCamera().changePos(CameraDirection::right, deltaTime);
  }
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    scene->getCamera().enableChangeDirection();
  }
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    scene->getCamera().disableChangeDirection();
  }
  if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
    scene->setLineModeOn();
  }
  if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
    scene->setFillModeOn();
  }

  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
    SceneManager::getInstance()->setCurrentScene("Basic");
  }
  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
    SceneManager::getInstance()->setCurrentScene("Lighting");
  }
  if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
    SceneManager::getInstance()->setCurrentScene("Normal");
  }
  if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
    SceneManager::getInstance()->setCurrentScene("Model");
  }
  if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
    SceneManager::getInstance()->setCurrentScene("Shadow");
  }
  if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
    SceneManager::getInstance()->setCurrentScene("PointShadow");
  }
  if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
    SceneManager::getInstance()->setCurrentScene("Deferred");
  }

  if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && 
    SceneManager::getInstance()->getCurrentScene()->getName() == "Normal") {
    auto objectShader = new Shader("shader/object_n.vert", "shader/object_n.frag");
    scene->setObjectShader(objectShader);
  }
  if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS &&
    SceneManager::getInstance()->getCurrentScene()->getName() == "Normal") {
    auto objectShader = new Shader("shader/object.vert", "shader/object.frag");
    scene->setObjectShader(objectShader);
  }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }
  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  SceneManager::getInstance()->getCurrentScene()->getCamera().changeDirection(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  SceneManager::getInstance()->getCurrentScene()->getCamera().changeZoom(yoffset);
}