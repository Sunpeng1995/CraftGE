#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

class Texture {
public:
  enum WrapType {
    Repeat,
    Mirrored_Repeat,
    Clamp
  };

  Texture(std::string filepath, std::string type);
  Texture(std::string filepath, std::string type, WrapType wraptype);

  inline GLuint getTextureID() {
    return mTex;
  }

  inline std::string getName() {
    return mType;
  }

  inline std::string getPath() {
    return mPath;
  }

private:
  GLuint mTex;
  std::string mType;
  std::string mPath;
};

#endif // !__TEXTURE_H__
