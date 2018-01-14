#include "Texture.h"

Texture::Texture(std::string filepath, std::string type) : 
  Texture(filepath, type, Texture::WrapType::Repeat)
{
}

Texture::Texture(std::string filepath, std::string type, WrapType wraptype): mType(type), mPath(filepath) {
  glGenTextures(1, &mTex);

  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
  if (data) {
    GLenum format;
    if (nrChannels == 1) {
      format = GL_RED;
    }
    else if (nrChannels == 3) {
      format = GL_RGB;
    }
    else if (nrChannels == 4) {
      format = GL_RGBA;
    }

    GLint wrap;
    switch (wraptype)
    {
    case Texture::Repeat:
      wrap = GL_REPEAT;
      break;
    case Texture::Mirrored_Repeat:
      wrap = GL_MIRRORED_REPEAT;
      break;
    case Texture::Clamp:
      wrap = GL_CLAMP_TO_EDGE;
      break;
    default:
      wrap = GL_REPEAT;
      break;
    }

    glBindTexture(GL_TEXTURE_2D, mTex);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }
  else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  glBindTexture(GL_TEXTURE_2D, 0);
}