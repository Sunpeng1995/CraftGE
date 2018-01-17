#include "Skybox.h"

Skybox::Skybox(std::string path) : mPath(path) {
  mShader = new Shader("shader/skybox.vert", "shader/skybox.frag");
  loadCubemap();

  glGenVertexArrays(1, &mVAO);
  glBindVertexArray(mVAO);

  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), skyboxVertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Skybox::setShader(Shader* shader) {
  Shader* old = mShader;
  mShader = shader;
  if (old) delete old;
}

void Skybox::loadCubemap() {
  glGenTextures(1, &mTextures);
  glBindTexture(GL_TEXTURE_CUBE_MAP, mTextures);

  int width, height, nrChannels;

  stbi_set_flip_vertically_on_load(false);

  for (int i = 0; i < FACES.size(); i++) {
    std::string str = mPath + "/" + FACES[i];
    std::cout << str << std::endl;
    unsigned char *data = stbi_load(str.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else {
      std::cout << "ERROR::SKYBOX::CUBEMAP_TEXTURE_LOAD_FAILED_AT_PATH: " << FACES[i] << std::endl;
    }
    stbi_image_free(data);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::draw(Camera* camera) {
  glDepthFunc(GL_LEQUAL);
  mShader->use();
  mShader->setInt("skybox", 0);
  mShader->setMat4("view", glm::mat4(glm::mat3(camera->view())));
  mShader->setMat4("projection", camera->projection());

  glBindVertexArray(mVAO);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, mTextures);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glDepthFunc(GL_LESS);
}