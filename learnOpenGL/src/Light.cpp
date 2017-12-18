#include "Light.h"

Light::Light() : Light(glm::vec3(0.2f), glm::vec3(0.5f), glm::vec3(1.0f)) {

}

Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
  mAmbient(ambient), mDiffuse(diffuse), mSpecular(specular), mMesh(nullptr)
{

}

void Light::setMesh(Mesh* mesh) {
  mMesh = mesh;
}

DirLight::DirLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
  Light(ambient, diffuse, specular), mDirection(direction)
{

}

void DirLight::passToShader(Shader* shader) {
  shader->setVec3("dirLight.direction", mDirection);
  shader->setVec3("dirLight.ambient", mAmbient);
  shader->setVec3("dirLight.diffuse", mDiffuse);
  shader->setVec3("dirLight.specular", mSpecular);
}

void DirLight::draw(Shader* shader) {

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
  mMesh->draw(shader);
}

SpotLight::SpotLight(int id, glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
  float cutoff, float outercutoff) :
  Light(ambient, diffuse, specular), mID(id), mPosition(position), mDirection(direction),
  mCutOff(cutoff), mOuterCutOff(outercutoff) 
{
  if (id >= 0) {
    mName = std::string("spotLight[");
    auto num = std::to_string(id);
    mName.append(num);
    mName.append("].");
  }
  else {
    mName = std::string("spotLight.");
  }
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