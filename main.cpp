#include "openglrenderer.h"

// Shader sources.
const GLchar* vertexSource =
    "#version 150\n"
    "in vec2 position;"
    "void main()"
    "{"
    "    gl_Position = vec4(position, 0.0, 1.0);" // remember that our vertex position is already specified as device coordinates.
    "}";
const GLchar* fragmentSource =
    "#version 150\n"
    "uniform vec3 triangleColor;" // uniforms are another to pass data to the shaders.
    "out vec4 outColor;" // the fragment shader has one mandatory output, the final color of a fragment.
    "void main()"
    "{"
    "    outColor = vec4(triangleColor, 1.0);"
    "}";
 
int main() {
  // GLFW needs to be initialized when the program starts.
  glfwInit();
  
  // Create and configure the window.
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr); // windowed mode.
  glfwMakeContextCurrent(window); // the OpenGL context has to be made active

  // Initialize GLEW.
  glewExperimental = GL_TRUE;
  glewInit();

  OpenGLRenderer renderer;

  renderer.buildVAO();

  renderer.buildProgramWithVertexSources(vertexSource, fragmentSource);
  
  renderer.setupParameters();
  
  auto t_start = std::chrono::high_resolution_clock::now();
  
  // The Event-Loop...
  while (!glfwWindowShouldClose(window)) {
    renderer.render(t_start);

    // These two functions are required.
    glfwSwapBuffers(window); // swaps the back buffer and front buffer after you've finished drawing.
    glfwPollEvents(); // retrieve window events.
  }

  // Clean up.
  renderer.deleteProgram();
  renderer.deleteBuffers();

  // Clean up when program closes.
  glfwTerminate();
}

