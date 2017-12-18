#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

class Texture {
public:
  Texture(std::string filepath, GLenum format);

  inline GLuint getTextureID() {
    return mTex;
  }

private:
  GLuint mTex;
};

#endif // !__TEXTURE_H__
