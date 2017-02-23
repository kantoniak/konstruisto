#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main() {

  // Test defines
  std::cout << "Hello, " << PROJECT_NAME << " " << BUILD_DESC << "!" << std::endl;
  #ifdef DEBUG
    std::cout << "It's debug." << std::endl;
  #endif

  // Test GLFW
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  GLint glewInitResult = glewInit();
  if (glewInitResult != GLEW_OK) {
    glfwTerminate();
    return -1;
  }

  glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}