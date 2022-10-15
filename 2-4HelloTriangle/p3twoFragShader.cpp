#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* firstFragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n\0";
const char* secondFragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n"
"}\n\0";

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, 800, 600);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


  int  success;
  char infoLog[512];

  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }


  const char* fragmentShaderSource[2] = {firstFragmentShaderSource, secondFragmentShaderSource};
  unsigned int fragmentShader[2];

  for (int i = 0; i < 2; ++i) {
    fragmentShader[i] = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader[i], 1, &fragmentShaderSource[i], NULL);
    glCompileShader(fragmentShader[i]);
    glGetShaderiv(fragmentShader[i], GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(fragmentShader[i], 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT" << i << "::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
  }


  unsigned int shaderProgram[2];
  for (int i = 0; i < 2; ++i) {
    shaderProgram[i] = glCreateProgram();
    glAttachShader(shaderProgram[i], vertexShader);
    glAttachShader(shaderProgram[i], fragmentShader[i]);
    glLinkProgram(shaderProgram[i]);
    glGetProgramiv(shaderProgram[i], GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(shaderProgram[i], 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM" << i << "::LINK_FAILED\n" << infoLog << std::endl;
    }
  }


  glDeleteShader(vertexShader);
  for (auto shader : fragmentShader) {
    glDeleteShader(shader);
  }


  float firstTriangle[] = {
       -0.5f, 0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f,
      -0.25f, 0.0f, 0.0f
  };
  float secondTriangle[] = {
           0.5f, 0.5f, 0.0f,
       0.5f, -0.5f, 0.0f,
      0.25f, 0.0f, 0.0f,
  };


  unsigned int VAOs[2], VBOs[2];
  glGenVertexArrays(2, VAOs);
  glGenBuffers(2, VBOs);


  glBindVertexArray(VAOs[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(VAOs[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(0);


  glBindBuffer(GL_ARRAY_BUFFER, 0);

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < 2; ++i) {
      glUseProgram(shaderProgram[i]);
      glBindVertexArray(VAOs[i]);
      glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(2, VAOs);
  glDeleteBuffers(2, VBOs);
  for (auto program : shaderProgram) {
    glDeleteProgram(program);
  }

  glfwTerminate();

  return 0;
}

void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  std::cout << "TEST" << std::endl;
  glViewport(0, 0, width, height);
}
