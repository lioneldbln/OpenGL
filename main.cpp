#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <cmath>

float vertices[] = {
  0.0f,  0.5f,
  0.5f, -0.5f,
 -0.5f, -0.5
};

// Shader sources
const GLchar* vertexSource =
    "#version 150\n"
    "in vec2 position;"
    "void main()"
    "{"
    "    gl_Position = vec4(position, 0.0, 1.0);"
    "}";
const GLchar* fragmentSource =
    "#version 150\n"
    "uniform vec3 triangleColor;"
    "out vec4 outColor;"
    "void main()"
    "{"
    "    outColor = vec4(triangleColor, 1.0);"
    "}";
 
int main() {
  glfwInit();
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);

  glfwMakeContextCurrent(window);

  // Initialize GLEW
  glewExperimental = GL_TRUE;
  glewInit();

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo;
  glGenBuffers(1, &vbo); // generate 1 buffer.
  glBindBuffer(GL_ARRAY_BUFFER, vbo); // make it the active object.

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Compiling Shaders.
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glBindFragDataLocation(shaderProgram, 0, "outColor");

  glLinkProgram(shaderProgram);

  glUseProgram(shaderProgram);
  
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");

  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glEnableVertexAttribArray(posAttrib);

  GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");
  
  auto t_start = std::chrono::high_resolution_clock::now();
  
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    auto t_now = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

    glUniform3f(uniColor, (std::sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
}

