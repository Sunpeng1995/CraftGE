#include "Light.h"

Light::Light() : Light(glm::vec3(0.2f), glm::vec3(0.5f), glm::vec3(1.0f)) {

}

Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
  mAmbient(ambient), mDiffuse(diffuse), mSpecular(specular), mLightMesh(nullptr)
{

}

void Light::setLightMesh(Mesh* mesh) {
  mLightMesh = mesh;
}

void Light::setAmbient(glm::vec3 ambient) {
  mAmbient = ambient;
}

void Light::setAmbient(float x, float y, float z) {
  mAmbient = glm::vec3(x, y, z);
}

void Light::setDiffuse(glm::vec3 diffuse) {
  mDiffuse = diffuse;
}

void Light::setDiffuse(float x, float y, float z) {
  mDiffuse = glm::vec3(x, y, z);
}

void Light::setSpecular(glm::vec3 specular) {
  mSpecular = specular;
}

void Light::setSpecular(float x, float y, float z) {
  mSpecular = glm::vec3(x, y, z);
}


/////////////////////////////////////////////

DirLight::DirLight(glm::vec3 direction) : Light(), mDirection(direction), mPosition(direction * -1.0f) {

}

DirLight::DirLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
  Light(ambient, diffuse, specular), mDirection(direction), mPosition(direction * -1.0f)
{

}

void DirLight::setDirection(glm::vec3 direction) {
  mDirection = direction;
}

void DirLight::setDirection(float x, float y, float z) {
  mDirection = glm::vec3(x, y, z);
}

void DirLight::passToShader(Shader* shader) {
  shader->setVec3("dirLight.direction", mDirection);
  shader->setVec3("dirLight.ambient", mAmbient);
  shader->setVec3("dirLight.diffuse", mDiffuse);
  shader->setVec3("dirLight.specular", mSpecular);
}

void DirLight::draw(Shader* shader) {

}


////////////////////////////////////////////////

PointLight::PointLight(int id) : PointLight(id, glm::vec3(0.0f)) {

}

PointLight::PointLight(int id, glm::vec3 position) : 
  PointLight(id, position, glm::vec3(0.2f), glm::vec3(0.5f), glm::vec3(1.0f), 1.0f, 0.09f, 0.0032f) {

}

PointLight::PointLight(int id, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
  float constant, float linear, float quadratic) :
  Light(ambient, diffuse, specular), mID(id), mPosition(position), mConstant(constant), mLinear(linear), mQuadratic(quadratic)
{
  if (id >= 0) {
    mName = std::string("pointLights[");
    auto num = std::to_string(id);
    mName.append(num);
    mName.append("].");
  }
  else {
    mName = std::string("pointLight.");
  }
}

void PointLight::setPosition(glm::vec3 pos) {
  mPosition = pos;
}

void PointLight::setPosition(float x, float y, float z) {
  mPosition = glm::vec3(x, y, z);
}

void PointLight::setConstant(float constant) {
  mConstant = constant;
}

void PointLight::setLinear(float linear) {
  mLinear = linear;
}

void PointLight::setQuadratic(float quadratic) {
  mQuadratic = quadratic;
}

void PointLight::passToShader(Shader* shader) {
  shader->setVec3(mName + "position", mPosition);
  shader->setVec3(mName + "ambient", mAmbient);
  shader->setVec3(mName + "diffuse", mDiffuse);
  shader->setVec3(mName + "specular", mSpecular);
  shader->setFloat(mName + "constant", mConstant);
  shader->setFloat(mName + "linear", mLinear);
  shader->setFloat(mName + "quadratic", mQuadratic);
}

void PointLight::draw(Shader* shader) {
  if (mLightMesh) {
    mLightMesh->draw(shader);
  }
}

SpotLight::SpotLight(int id) :
  SpotLight(id, glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f))) 
{}

SpotLight::SpotLight(int id, glm::vec3 position, glm::vec3 direction, float cutoff, float outercutoff) :
  Light(), mID(id), mPosition(position), mDirection(position), mCutOff(cutoff), mOuterCutOff(outercutoff) 
{

}

SpotLight::SpotLight(int id, glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
  float cutoff, float outercutoff) :
  Light(ambient, diffuse, specular), mID(id), mPosition(position), mDirection(direction),
  mCutOff(cutoff), mOuterCutOff(outercutoff) 
{
  if (id >= 0) {
    mName = std::string("spotLights[");
    auto num = std::to_string(id);
    mName.append(num);
    mName.append("].");
  }
  else {
    mName = std::string("spotLight.");
  }
}

void SpotLight::setPosition(glm::vec3 pos) {
  mPosition = pos;
}

void SpotLight::setPosition(float x, float y, float z) {
  mPosition = glm::vec3(x, y, z);
}

void SpotLight::setDirection(glm::vec3 dir) {
  mDirection = dir;
}

void SpotLight::setDirection(float x, float y, float z) {
  mDirection = glm::vec3(x, y, z);
}

void SpotLight::setCutOff(float cutoff) {
  mCutOff = cutoff;
}

void SpotLight::setOuterCutOff(float outercutoff) {
  mOuterCutOff = outercutoff;
}

void SpotLight::passToShader(Shader* shader) {
  shader->setVec3(mName + "position", mPosition);
  shader->setVec3(mName + "direction", mDirection);
  shader->setVec3(mName + "ambient", mAmbient);
  shader->setVec3(mName + "diffuse", mDiffuse);
  shader->setVec3(mName + "specular", mSpecular);
  shader->setFloat(mName + "cutOff", mCutOff);
  shader->setFloat(mName + "outerCutOff", mOuterCutOff);
}

void SpotLight::draw(Shader* shader) {

}

void SpotLight::updateFlashLight(glm::vec3 position, glm::vec3 direction) {
  mPosition = position;
  mDirection = direction;
}