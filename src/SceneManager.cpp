#include "SceneManager.h"

SceneManager* SceneManager::gSceneManager = nullptr;

SceneManager::SceneManager() {

}

SceneManager::~SceneManager() {
  for (auto i : mLoadedScenes) {
    delete i;
  }
  mLoadedScenes.clear();
}

SceneManager* SceneManager::getInstance() {
  if (gSceneManager == nullptr) {
    gSceneManager = new SceneManager();
  }
  return gSceneManager;
}

void SceneManager::addScene(Scene* scene) {
  mLoadedScenes.push_back(scene);
}

Scene* SceneManager::findScene(std::string scene_name) {
  for (auto i : mLoadedScenes) {
    if (i->getName() == scene_name) {
      return i;
    }
  }
  return nullptr;
}

void SceneManager::setCurrentScene(std::string scene_name) {
  auto scene = findScene(scene_name);
  if (scene != nullptr) {
    mCurrentScene = scene;
  }
}

void SceneManager::setCurrentScene(Scene* scene) {
  auto s = findScene(scene->getName());
  if (s == nullptr) {
    addScene(scene);
  }
  mCurrentScene = scene;
}

Scene* SceneManager::getCurrentScene() {
  return mCurrentScene;
}

void SceneManager::setScreenWidth(unsigned int width) {
  mScreenWidth = width;
  updateScreenSize();
}

void SceneManager::setScreenHeight(unsigned int height) {
  mScreenHeight = height;
  updateScreenSize();
}

void SceneManager::setScreenSize(unsigned int width, unsigned int height) {
  mScreenWidth = width;
  mScreenHeight = height;
  updateScreenSize();
}

void SceneManager::updateScreenSize() {
  for (auto i : mLoadedScenes) {
    i->updateScreen(mScreenWidth, mScreenHeight);
  }
}

void SceneManager::createBasicScene() {
  Scene* basicScene = new Scene(mScreenWidth, mScreenHeight, "Basic");

  Texture tex1("res/textures/container.jpg", "normal");
  Texture tex2("res/textures/awesomeface.png", "normal");

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

  addScene(basicScene);
}

void SceneManager::createLightingScene() {
  Scene* lightingScene = new Scene(mScreenWidth, mScreenHeight, "Lighting");

  auto objectShader = new Shader("shader/object.vert", "shader/object.frag");
  auto lightingShader = new Shader("shader/lighting.vert", "shader/lighting.frag");
  Texture tex1("res/textures/container2.png", "texture_diffuse");
  Texture tex2("res/textures/container2_specular.png", "texture_specular");

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
  //lightingScene->setFlashLight(spotLight);

  lightingScene->setLightingShader(lightingShader);

  addScene(lightingScene);
}

void SceneManager::createModelScene() {
  auto ModelScene = new Scene(mScreenWidth, mScreenHeight, "Model");

  auto objectShader = new Shader("shader/object.vert", "shader/object.frag");
  auto lightingShader = new Shader("shader/lighting.vert", "shader/lighting.frag");

  //Model* nanosuit = new Model("obj/nanosuit/nanosuit.obj");
  //nanosuit->setPosition(glm::vec3(0.0f, -1.0f, 0.0f));
  //nanosuit->setScale(0.1f);
  //ModelScene->addModel(nanosuit);

  Model* cruiser = new Model("res/models/cruiser/cruiser.obj");
  cruiser->setPosition(glm::vec3(2.0f, 0.0f, 0.0f));
  ModelScene->addModel(cruiser);

  Model* f16 = new Model("res/models/f16/f16.obj");
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

  addScene(ModelScene);
}

void SceneManager::createNormalScene() {
  Scene* lightingScene = new Scene(mScreenWidth, mScreenHeight, "Normal");

  auto objectShader = new Shader("shader/object_n.vert", "shader/object_n.frag");
  auto lightingShader = new Shader("shader/lighting.vert", "shader/lighting.frag");

  lightingScene->setObjectShader(objectShader);

  for (int i = 0; i < 10; i++) {
    Model* m = new Model("res/models/normalcube/cube.obj");
    m->setPosition(cubePositions[i]);
    lightingScene->addModel(m);
    m->updateRegister([=](Object* o){
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

  addScene(lightingScene);
}

void SceneManager::createShadowScene() {
  Scene* shadowScene = new Scene(mScreenWidth, mScreenHeight, "Shadow");
  auto objectShader = new Shader("shader/object.vert", "shader/object.frag");
  auto lightingShader = new Shader("shader/lighting.vert", "shader/lighting.frag");
  Texture tex1("res/textures/wood.png", "texture_diffuse");
  Texture tex2("res/textures/wood.png", "texture_specular");

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

  addScene(shadowScene);
}

void SceneManager::createPointShadowScene() {
  auto pointShadowScene = new Scene(mScreenWidth, mScreenHeight, "PointShadow");
  auto lightingShader = new Shader("shader/lighting.vert", "shader/lighting.frag");
  Texture tex1("res/textures/wood.png", "texture_diffuse");
  Texture tex2("res/textures/wood.png", "texture_specular");

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

  addScene(pointShadowScene);
}

void SceneManager::createDeferredShadingScene() {
  auto ds_scene = new Scene(mScreenWidth, mScreenHeight, "Deferred", Scene::deferred);

  for (int i = 0; i < 9; i++) {
    Model* nano = new Model("res/models/nanosuit/nanosuit.obj");
    //Model* nano = new Model("obj/f16/f16.obj");
    nano->setPosition(objectPositions[i]);
    nano->setScale(0.2f);
    ds_scene->addModel(nano);
  }

  int lightCount = 128;
  std::vector<glm::vec3> lightPositions;
  std::vector<glm::vec3> lightColors;
  srand(13);
  for (int i = 0; i < lightCount; i++) {
    float x = ((rand() % 100) / 100.0f) * 6 - 3.0;
    float y = ((rand() % 100) / 100.0f) * 6 - 3.5;
    float z = ((rand() % 100) / 100.0f) * 6 - 3.0;
    lightPositions.push_back(glm::vec3(x, y, z));
    float r = ((rand() % 100) / 200.0f) + 0.5;
    float g = ((rand() % 100) / 200.0f) + 0.5;
    float b = ((rand() % 100) / 200.0f) + 0.5;
    lightColors.push_back(glm::vec3(r, g, b));
  }

  for (int i = 0; i < lightCount; i++) {
    auto pointLight = new PointLight(i, lightPositions[i], glm::vec3(0.1f), lightColors[i], lightColors[i], 1.0, 0.7, 1.8);
    auto lightcube = new Cube(lightPositions[i]);
    lightcube->setScale(0.1f);
    pointLight->setLightMesh(lightcube);
    ds_scene->addOtherLight(pointLight);
  }

  auto lightingShader = new Shader("shader/lighting.vert", "shader/lighting.frag");
  ds_scene->setLightingShader(lightingShader);

  addScene(ds_scene);
}

void SceneManager::createFoggedScene() {
  Scene* shadowScene = new Scene(mScreenWidth, mScreenHeight, "Fogged", Scene::deferred);
  auto lightingShader = new Shader("shader/lighting.vert", "shader/lighting.frag");
  Texture tex1("res/textures/wood.png", "texture_diffuse");
  Texture tex2("res/textures/wood.png", "texture_specular");
  Texture noise("res/fog_noise.jpg", "fogNoise");
  shadowScene->addOtherTextures(noise);

  glm::vec3 cubepos[] = {
    glm::vec3(0.0f, 1.5f, 0.0f),
    glm::vec3(2.0f, 0.0f, 1.0f),
    glm::vec3(-1.0f, 0.0f, 2.0f)
  };

  for (int i = 0; i < 3; i++) {
    NormalledCube* obj = new NormalledCube(cubepos[i]);
    obj->addTexture(tex1);
    obj->addTexture(tex2);
    obj->setScale(0.25f, 1.0f, 0.25f);
    if (i == 2) {
      obj->setScale(0.125f, 0.5f, 0.125f);
    }
    shadowScene->addMesh(obj);
  }
  Plane* plane = new Plane(glm::vec3(0.0f, 0.25f, 0.0f));
  plane->addTexture(tex1);
  shadowScene->addMesh(plane);
  //Plane* p2 = new Plane(glm::vec3(2.0f, 0.25f, 2.0f));
  //p2->addTexture(tex1);
  //shadowScene->addMesh(p2);

  auto dirLight = new DirLight(glm::vec3(2.0f, -4.0f, 1.0f));
  shadowScene->setDirLight(dirLight);

  shadowScene->setLightingShader(lightingShader);

  addScene(shadowScene);
}

void SceneManager::createParticlesScene() {
    auto particle_scene = new Scene(mScreenWidth, mScreenHeight, "Particles");
    auto shader = new Shader("shader/particles/particles.vert", "shader/particles/particles.frag");
    auto rain_shader = new Shader("shader/particles/rain.vert", "shader/particles/rain.frag");

    auto particles = new ParticleSystem("particles", 10000, glm::vec3(0, 3.0f, -3.0f));

    particles->setParticleTexture(Texture("res/textures/rain_texture.png", "particle"));

    particles->setGenerateSpeed(1000);
    particles->setSpeedDir(glm::vec3(0, -1.0f, 0));
    particles->setSpeedDirVar(0.0f);
    particles->setSpeedNorm(2.0f);
    particles->setLifeTime(2.5f);
    particles->setSpawnBoxSize(15.0f, 0.5f, 15.0f);
    particles->setGravityNorm(0.0f);


    particle_scene->addObject(particles);
    particle_scene->setObjectShader(rain_shader);
    particles->setCamera(particle_scene->getCameraPointer());

    addScene(particle_scene);
}

void SceneManager::createParticlesAnimateScene() {
    auto particle_scene = new Scene(mScreenWidth, mScreenHeight, "ParticlesAnimate");
    auto animate_shader = new Shader("shader/particles/particles_mult_period.vert", "shader/particles/particles_mult_period.frag");

    auto particles_animate = new ParticleSystem("particles", 1000, glm::vec3(0, 0, 0.0f));

    particles_animate->setParticleTexture(Texture("res/textures/ParticleAtlas.png", "particle"));

    particles_animate->setGenerateSpeed(1);
    particles_animate->setSpeedNorm(0);
    particles_animate->setLifeTime(1.0f);
    particles_animate->setLifeTimeVar(0);
    particles_animate->setSpawnBoxSize(2.0f, 2.0f, 0.5f);
    particles_animate->setGravityNorm(0);
    particles_animate->setTexturePeriod(64);

    particles_animate->setCamera(particle_scene->getCameraPointer());

    particle_scene->addObject(particles_animate);
    particle_scene->setObjectShader(animate_shader);

    addScene(particle_scene);
}

void SceneManager::createAllExampleScenes() {
  createBasicScene();
  createLightingScene();
  createModelScene();
  createNormalScene();
  createShadowScene();
  createPointShadowScene();
  createDeferredShadingScene();
  createFoggedScene();
  createParticlesScene();
  createParticlesAnimateScene();

  setCurrentScene("Basic");
}
