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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
Scene* createBasicScene();
Scene* createLightingScene();

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float sWidth = 800.0f;
float sHeight = 600.0f;

bool firstMouse = true;
float lastX, lastY;

glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};

Scene* scene;

int main() {
  // init glfw
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // create a glfw window
  GLFWwindow* window = glfwCreateWindow(sWidth, sHeight, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create glfw window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // init glad before calling any OpenGL function
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  glEnable(GL_DEPTH_TEST);

  //scene = createBasicScene();
  scene = createLightingScene();

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    scene->draw();


    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}

Scene* createBasicScene() {
  Scene* basicScene = new Scene(sWidth, sHeight);

  Texture* tex1 = new Texture("res/container.jpg", GL_RGB);
  Texture* tex2 = new Texture("res/awesomeface.png", GL_RGBA);


  for (int i = 0; i < 10; i++) {
    Cube* cube = new Cube(cubePositions[i]);
    cube->setRotate(i * 10.0f, glm::vec3(0.5f, 1.0f, 0.0f));
    cube->addTexture(tex1);
    cube->addTexture(tex2);
    basicScene->addMesh(cube);
  }

  return basicScene;
}

Scene* createLightingScene() {
  Scene* lightingScene = new Scene(sWidth, sHeight);

  auto objectShader = new Shader("shader/object.vert", "shader/object.frag");
  auto lightingShader = new Shader("shader/lighting.vert", "shader/lighting.frag");
  Texture* tex1 = new Texture("res/container2.png", GL_RGBA);
  Texture* tex2 = new Texture("res/container2_specular.png", GL_RGBA);

  for (int i = 0; i < 10; i++) {
    NormalledCube* obj = new NormalledCube(cubePositions[i]);
    obj->setRotate(i * 10.0f, glm::vec3(0.5f, 1.0f, 0.0f));
    obj->addTexture(tex1);
    obj->addTexture(tex2);
    lightingScene->addMesh(obj);
  }
  lightingScene->setObjectShader(objectShader);

  glm::vec3 ambient(0.1f, 0.1f, 0.1f);
  glm::vec3 diffuse(0.5f, 0.5f, 0.5f);
  glm::vec3 specular(1.0f, 1.0f, 1.0f);

  glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f,  0.2f,  2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3(0.0f,  0.0f, -3.0f)
  };

  auto dirLight = new DirLight(glm::vec3(-0.2f, -1.0f, -0.3f), ambient, diffuse, specular);
  lightingScene->setDirLight(dirLight);

  for (int i = 0; i < 4; i++) {
    auto pointLight = new PointLight(i, pointLightPositions[i], ambient, diffuse, specular, 1.0f, 0.09f, 0.032f);
    Cube* light = new Cube(pointLightPositions[i]);
    light->setScale(0.2f);
    pointLight->setMesh(light);
    lightingScene->addOtherLight(pointLight);
  }

  auto spotLight = new SpotLight(-1, lightingScene->getCamera().getPos(), lightingScene->getCamera().getFront(),
    ambient, diffuse, specular, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)));
  lightingScene->setFlashLight(spotLight);

  lightingScene->setLightingShader(lightingShader);

  return lightingScene;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
  scene->updateScreen(width, height);
}

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

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

  scene->getCamera().changeDirection(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  scene->getCamera().changeZoom(yoffset);
}