#include "Shader.h"

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vertFile;
  std::ifstream fragFile;

  vertFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fragFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    vertFile.open(vertexPath);
    fragFile.open(fragmentPath);
    std::stringstream vStream, fStream;
    vStream << vertFile.rdbuf();
    fStream << fragFile.rdbuf();
    vertFile.close();
    fragFile.close();
    vertexCode = vStream.str();
    fragmentCode = fStream.str();
  }
  catch (std::ifstream::failure e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
  }

  const char* vCode = vertexCode.c_str();
  const char* fCode = fragmentCode.c_str();



  unsigned int vertexShader, fragmentShader;
  int success;
  char infoLog[512];

  // Compile vertex shader
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vCode, NULL);
  glCompileShader(vertexShader);

  // Check if the shader compile successfully
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // Compile fragment shader
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fCode, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // Create and link a shader program
  ID = glCreateProgram();
  glAttachShader(ID, vertexShader);
  glAttachShader(ID, fragmentShader);
  glLinkProgram(ID);

  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}
