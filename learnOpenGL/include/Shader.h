#ifndef __SHADER_H__
#define __SHADER_H__

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <streambuf>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:

  Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

  inline void use() {
    glUseProgram(ID);
  }
  inline unsigned int getShaderID() {
    return ID;
  }
  inline void setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
  }
  inline void setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  }
  inline void setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
  }
  inline void setMat4(const std::string &name, glm::mat4 value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
  }

private:
  unsigned int ID;

  //inline bool findUniformLocation(const std::string &name) const{
  //  int exist = glGetUniformLocation(ID, name.c_str());
  //  if (exist == -1) {
  //    return false;
  //  }
  //  return true;
  //}
};



#endif // !__SHADER_H__
