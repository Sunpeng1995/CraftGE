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

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float sWidth = 1280.0f;
float sHeight = 720.0f;

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

Scene* scene, *basicScene, *lightingScene, *normalScene, *modelScene, *shadowScene, *pointShadowScene;
int currentScene = 1;
bool isNormal = true;

int main() {
  // init glfw
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwWindowHint(GLFW_SAMPLES, 4);

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
  glViewport(0, 0, sWidth, sHeight);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  basicScene = createBasicScene();
  lightingScene = createLightingScene();
  normalScene = createNormalScene();
  modelScene = createModelScene();
  shadowScene = createShadowScene();
  pointShadowScene = createPointShadowScene();

  scene = basicScene;

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    scene->update();
    scene->draw();


    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}

Scene* createBasicScene() {
  Scene* basicScene = new Scene(sWidth, sHeight);

  Texture tex1("res/container.jpg", "normal");
  Texture tex2("res/awesomeface.png", "normal");

  //basicScene->setSkybox(new Skybox("res/skybox"));

  Cube* cube = new Cube(cubePositions[0]);
  cube->addTexture(tex1);
  cube->addTexture(tex2);
  cube->setScale(0.5f);
  cube->updateRegister([](Object* o) {
    glm::vec3 rotate = o->getRotation();
    rotate.y = rotate.y + 0.004f;
    if (rotate.y > 360.0f) {
      rotate.y = rotate.y - 360.0f;
    }
    o->setRotation(rotate);
    //glm::vec3 position = glm::vec3(0, glm::sin((float)glfwGetTime()), 0);
    //o->setPosition(position);
    float scale = (glm::sin(glfwGetTime()) + 1.0f) / 4.0f + 0.5f;
    o->setScale(scale);
  });

  Cube* subCube = new Cube(cubePositions[0]);
  subCube->addTexture(tex1);
  subCube->addTexture(tex2);
  subCube->setPosition(glm::vec3(5.0f, 0, 0));
  subCube->setScale(0.5f);
  subCube->updateRegister([](Object* o) {
    glm::vec3 rotate = o->getRotation();
    rotate.x = rotate.x + 0.002f;
    if (rotate.x > 360.0f) {
      rotate.x = rotate.x - 360.0f;
    }
    o->setRotation(rotate);
  });

  Cube* subSubCube = new Cube(cubePositions[0]);
  subSubCube->addTexture(tex1);
  subSubCube->addTexture(tex2);
  subSubCube->setPos(glm::vec3(0, 5.0f, 0));
  subSubCube->setScale(0.5f);

  cube->addChild(subCube);
  subCube->addChild(subSubCube);

  basicScene->addMesh(cube);

  return basicScene;
}

Scene* createLightingScene() {
  Scene* lightingScene = new Scene(sWidth, sHeight);

  auto objectShader = new Shader("shader/object.vert", "shader/object.frag");
  auto lightingShader = new Shader("shader/lighting.vert", "shader/lighting.frag");
  Texture tex1("res/container2.png", "texture_diffuse");
  Texture tex2("res/container2_specular.png", "texture_specular");

  for (int i = 0; i < 10; i++) {
    NormalledCube* obj = new NormalledCube(cubePositions[i]);
    obj->addTexture(tex1);
    obj->addTexture(tex2);
    obj->setScale(0.5f);
    lightingScene->addMesh(obj);
    obj->updateRegister([=](Object* o) {
      glm::vec3 rotate = o->getRotation();
      rotate.y = rotate.y + 0.0005f * i;
      rotate.x = rotate.x + 0.00005f * i;
      if (rotate.y > 360.0f) {
        rotate.y = rotate.y - 360.0f;
      }
      if (rotate.x > 360.0f) {
        rotate.x = rotate.x - 360.0f;
      }
      o->setRotation(rotate);
    });
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
    pointLight->setLightMesh(light);
    lightingScene->addOtherLight(pointLight);
  }

  auto spotLight = new SpotLight(-1, lightingScene->getCamera().getPos(), lightingScene->getCamera().getFront(),
    ambient, diffuse, specular, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)));
  lightingScene->setFlashLight(spotLight);

  lightingScene->setLightingShader(lightingShader);

  return lightingScene;
}

Scene* createModelScene() {
  auto ModelScene = new Scene(sWidth, sHeight);

  auto objectShader = new Shader("shader/object.vert", "shader/object.frag");
  auto lightingShader = new Shader("shader/lighting.vert", "shader/lighting.frag");

  //Model* nanosuit = new Model("obj/nanosuit/nanosuit.obj");
  //nanosuit->setPosition(glm::vec3(0.0f, -1.0f, 0.0f));
  //nanosuit->setScale(0.1f);
  //ModelScene->addModel(nanosuit);

  Model* cruiser = new Model("obj/cruiser/cruiser.obj");
  cruiser->setPosition(glm::vec3(2.0f, 0.0f, 0.0f));
  ModelScene->addModel(cruiser);

  Model* f16 = new Model("obj/f16/f16.obj");
  f16->setPosition(glm::vec3(-2.0f, 0.0f, 0.0f));
  ModelScene->addModel(f16);

  ModelScene->setObjectShader(objectShader);

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
  ModelScene->setDirLight(dirLight);

  for (int i = 0; i < 4; i++) {
    auto pointLight = new PointLight(i, pointLightPositions[i], ambient, diffuse, specular, 1.0f, 0.09f, 0.032f);
    Cube* light = new Cube(pointLightPositions[i]);
    light->setScale(0.2f);
    pointLight->setLightMesh(light);
    ModelScene->addOtherLight(pointLight);
  }

  ModelScene->setLightingShader(lightingShader);

  return ModelScene;
}

Scene* createNormalScene() {
  Scene* lightingScene = new Scene(sWidth, sHeight);

  auto objectShader = new Shader("shader/object_n.vert", "shader/object_n.frag");
  auto lightingShader = new Shader("shader/lighting.vert", "shader/lighting.frag");
  Texture tex1("res/brick_d.jpg", "texture_diffuse");
  Texture tex2("res/brick_n.jpg", "texture_normal");

  for (int i = 0; i < 10; i++) {
    NormalledCube* obj = new NormalledCube(cubePositions[i]);
    obj->addTexture(tex1);
    obj->addTexture(tex2);
    obj->setScale(0.5f);
    lightingScene->addMesh(obj);
    obj->updateRegister([=](Object* o) {
      glm::vec3 rotate = o->getRotation();
      rotate.y = rotate.y + 0.0005f * i;
      rotate.x = rotate.x + 0.00005f * i;
      if (rotate.y > 360.0f) {
        rotate.y = rotate.y - 360.0f;
      }
      if (rotate.x > 360.0f) {
        rotate.x = rotate.x - 360.0f;
      }
      o->setRotation(rotate);
    });
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
    pointLight->setLightMesh(light);
    lightingScene->addOtherLight(pointLight);
  }

  lightingScene->setLightingShader(lightingShader);

  return lightingScene;
}

Scene* createShadowScene() {
  Scene* shadowScene = new Scene(sWidth, sHeight);
  auto objectShader = new Shader("shader/object.vert", "shader/object.frag");
  auto lightingShader = new Shader("shader/lighting.vert", "shader/lighting.frag");
  Texture tex1("res/wood.png", "texture_diffuse");
  Texture tex2("res/wood.png", "texture_specular");

  glm::vec3 cubepos[] = {
    glm::vec3(0.0f, 1.5f, 0.0f),
    glm::vec3(2.0f, 0.0f, 1.0f),
    glm::vec3(-1.0f, 0.0f, 2.0f)
  };

  for (int i = 0; i < 3; i++) {
    NormalledCube* obj = new NormalledCube(cubepos[i]);
    //obj->setRotate(i * 10.0f, glm::vec3(0.5f, 1.0f, 0.0f));
    obj->addTexture(tex1);
    obj->addTexture(tex2);
    obj->setScale(0.25f);
    if (i == 2) {
      obj->setScale(0.125f);
    }
    shadowScene->addMesh(obj);
  }
  Plane* plane = new Plane(glm::vec3(0.0f, 0.25f, 0.0f));
  plane->addTexture(tex1);
  //plane->addTexture(tex2);
  shadowScene->addMesh(plane);
  shadowScene->setObjectShader(objectShader);

  auto dirLight = new DirLight(glm::vec3(2.0f, -4.0f, 1.0f));
  shadowScene->setDirLight(dirLight);
  shadowScene->setLightingShader(lightingShader);

  shadowScene->enableShadow(Scene::ShadowType::parallel);

  return shadowScene;
}

Scene* createPointShadowScene() {
  auto pointShadowScene = new Scene(sWidth, sHeight);
  auto lightingShader = new Shader("shader/lighting.vert", "shader/lighting.frag");
  Texture tex1("res/wood.png", "texture_diffuse");
  Texture tex2("res/wood.png", "texture_specular");

  glm::vec3 cubepos[] = {
    glm::vec3(4.0f, -3.5f, 0.0f),
    glm::vec3(2.0f, 3.0f, 1.0f),
    glm::vec3(-3.0f, -1.0f, 0.0f),
    glm::vec3(-1.5f, 1.0f, 1.5f),
    glm::vec3(-1.5f, 2.0f, -3.0f)
  };

  auto room = new NormalledCube(glm::vec3(0.0f, 0.0f, 0.0f));
  room->addTexture(tex1);
  room->addTexture(tex2);
  room->setScale(5.0f);
  room->setReverse(true);
  pointShadowScene->addMesh(room);

  for (int i = 0; i < 5; i++) {
    NormalledCube* obj = new NormalledCube(cubepos[i]);
    //obj->setRotate(i * 10.0f, glm::vec3(0.5f, 1.0f, 0.0f));
    obj->addTexture(tex1);
    obj->addTexture(tex2);
    obj->setScale(0.5f);
    if (i == 1 || i == 4) {
      obj->setScale(0.75f);
    }
    pointShadowScene->addMesh(obj);
  }

  auto pointLight = new PointLight(-1, glm::vec3(0.0f, 0.0f, 0.0f));
  pointLight->updateRegister([](Object* o) {
    // TODO: fix bug when light move

    //glm::vec3 position = glm::vec3(0, 0, glm::sin((float)glfwGetTime()) * 0.25f);
    //o->setPosition(position);
  });

  pointShadowScene->addOtherLight(pointLight);
  pointShadowScene->setLightingShader(lightingShader);

  pointShadowScene->enableShadow(Scene::ShadowType::point);

  return pointShadowScene;
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

  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && currentScene != 1) {
    scene = basicScene;
    currentScene = 1;
  }
  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && currentScene != 2) {
    scene = lightingScene;
    currentScene = 2;
  }
  if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && currentScene != 3) {
    scene = normalScene;
    currentScene = 3;
  }
  if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && currentScene != 4) {
    scene = modelScene;
    currentScene = 4;
  }
  if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && currentScene != 5) {
    scene = shadowScene;
    currentScene = 5;
  }
  if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && currentScene != 6) {
    scene = pointShadowScene;
    currentScene = 6;
  }

  if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && currentScene == 3) {
    auto objectShader = new Shader("shader/object_n.vert", "shader/object_n.frag");
    scene->setObjectShader(objectShader);
  }
  if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && currentScene == 3) {
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

  scene->getCamera().changeDirection(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  scene->getCamera().changeZoom(yoffset);
}