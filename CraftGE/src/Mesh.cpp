#include "Mesh.h"

Mesh::Mesh() {

}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) :
  mVertices(vertices), mIndices(indices), mTextures(textures), mScale(1.0f)
{
  glGenVertexArrays(1, &mVAO);
  glGenBuffers(1, &mVBO);
  glGenBuffers(1, &mEBO);

  glBindVertexArray(mVAO);
  glBindBuffer(GL_ARRAY_BUFFER, mVBO);
  glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::draw(Shader* shader) {
  glm::mat4 model;
  model = glm::translate(model, mPos);
  model = glm::scale(model, glm::vec3(mScale, mScale, mScale));
  shader->setMat4("model", model);

  unsigned int diffuseNr = 0;
  unsigned int specularNr = 0;
  for (unsigned int i = 0; i < mTextures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    std::stringstream ss;
    std::string name(mTextures[i].getName());
    if (name == "texture_diffuse") ss << diffuseNr++;
    if (name == "texture_specular") ss << specularNr++;
    
    shader->setInt("material." + name + ss.str(), i);
    glBindTexture(GL_TEXTURE_2D, mTextures[i].getTextureID());
  }
  glActiveTexture(GL_TEXTURE0);

  glBindVertexArray(mVAO);
  glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::addTexture(Texture tex) {
  mTextures.push_back(tex);
}

void Mesh::setScale(float scale) {
  mScale = scale;
}

void Mesh::setPos(glm::vec3 pos) {
  mPos = pos;
}

Cube::Cube() : Cube(glm::vec3(0.0f, 0.0f, 0.0f)) {
}

Cube::Cube(glm::vec3 pos) : mRotateAngle(0.0f), mRotateAxis(glm::vec3(0.0f, 1.0f, 0.0f)) {
  const float vertices[] = {
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f, 0.0f, 0.0f  // bottom-left  
  };


  mPos = pos;
  mScale = 1.0f;

  glGenVertexArrays(1, &mVAO);
  glBindVertexArray(mVAO);

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Cube::draw(Shader* shader) {
  glm::mat4 model;
  model = glm::translate(model, mPos);
  model = glm::rotate(model, (float)glfwGetTime() * glm::radians(mRotateAngle), mRotateAxis);
  model = glm::scale(model, glm::vec3(mScale, mScale, mScale));
  shader->setMat4("model", model);


  unsigned int textureNr = 0;
  for (unsigned int i = 0; i < mTextures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    
    shader->setInt("texture" + std::to_string(i), i);
    glBindTexture(GL_TEXTURE_2D, mTextures[i].getTextureID());
  }
  glActiveTexture(GL_TEXTURE0);

  glBindVertexArray(mVAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}

void Cube::setRotate(float angle, glm::vec3 axis) {
  mRotateAngle = angle;
  mRotateAxis = axis;
}


NormalledCube::NormalledCube() : NormalledCube(glm::vec3(0.0f, 0.0f, 0.0f)) {

}

NormalledCube::NormalledCube(glm::vec3 pos) : mRotateAngle(0.0f), mRotateAxis(glm::vec3(1.0f)) {
  float vertices[] = {
    // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left  
  };
  
  mPos = pos;
  mScale = 1.0f;

  glGenVertexArrays(1, &mVAO);
  glBindVertexArray(mVAO);

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void NormalledCube::draw(Shader* shader) {
  glm::mat4 model;
  model = glm::translate(model, mPos);
  model = glm::rotate(model, (float)glfwGetTime() * glm::radians(mRotateAngle), mRotateAxis);
  model = glm::scale(model, glm::vec3(mScale, mScale, mScale));
  shader->setMat4("model", model);

  unsigned int diffuseNr = 0;
  unsigned int specularNr = 0;
  unsigned int normalNr = 0;
  for (unsigned int i = 0; i < mTextures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    std::stringstream ss;
    std::string name(mTextures[i].getName());
    if (name == "texture_diffuse") ss << diffuseNr++;
    if (name == "texture_specular") ss << specularNr++;
    if (name == "texture_normal") ss << normalNr++;
    
    shader->setInt("material." + name + ss.str(), i);
    glBindTexture(GL_TEXTURE_2D, mTextures[i].getTextureID());
  }
  glActiveTexture(GL_TEXTURE0);

  if (mReverse) {
    glDisable(GL_CULL_FACE);
    shader->setBool("reverse_normals", true);
  }
  else {
    shader->setBool("reverse_normals", false);
  }
  glBindVertexArray(mVAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  if (mReverse) {
    glEnable(GL_CULL_FACE);
  }
}

void NormalledCube::setRotate(float angle, glm::vec3 axis) {
  mRotateAngle = angle;
  mRotateAxis = axis;
}


Plane::Plane() {
}

Plane::Plane(glm::vec3 pos) {
  GLfloat planeVertices[] = {
    // Positions          // Normals         // Texture Coords
    25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
    -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,
    -25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

    25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
    25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 25.0f,
    -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f
  };

  mPos = pos;
  mScale = 1.0f;
  glGenVertexArrays(1, &mVAO);
  glBindVertexArray(mVAO);

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Plane::draw(Shader* shader) {
  glm::mat4 model;
  model = glm::translate(model, mPos);
  model = glm::scale(model, glm::vec3(mScale, mScale, mScale));
  shader->setMat4("model", model);

  unsigned int diffuseNr = 0;
  unsigned int specularNr = 0;
  unsigned int normalNr = 0;
  for (unsigned int i = 0; i < mTextures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    std::stringstream ss;
    std::string name(mTextures[i].getName());
    if (name == "texture_diffuse") ss << diffuseNr++;
    if (name == "texture_specular") ss << specularNr++;
    if (name == "texture_normal") ss << normalNr++;
    
    shader->setInt("material." + name + ss.str(), i);
    glBindTexture(GL_TEXTURE_2D, mTextures[i].getTextureID());
  }
  glActiveTexture(GL_TEXTURE0);

  glBindVertexArray(mVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}