#ifndef __SHADOW_H__
#define __SHADOW_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Shadow {
public:
  Shadow();

  void draw();

private:
  const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

  GLuint mDepthMapFBO;
  GLuint mDepthMap;
};

#endif // !__SHADOW_H__
