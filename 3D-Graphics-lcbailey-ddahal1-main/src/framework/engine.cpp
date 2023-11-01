#include "engine.h"

Engine::Engine() : keys(), cameraZ(-3.0f) {
  this->initWindow();
  this->initShaders();
  this->initShapes();
  this->initMatrices();
}

Engine::~Engine() {}

unsigned int Engine::initWindow(bool debug) {
  // glfw: initialize and configure
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif
  glfwWindowHint(GLFW_RESIZABLE, false);

  window = glfwCreateWindow(width, height, "engine", nullptr, nullptr);
  glfwMakeContextCurrent(window);

  // glad: load all OpenGL function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    cout << "Failed to initialize GLAD" << endl;
    return -1;
  }

  // OpenGL configuration
  glViewport(0, 0, width, height);
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glfwSwapInterval(1);

  return 0;
}

void Engine::initShaders() {
  // load shader manager
  shaderManager = make_unique<ShaderManager>();

  // Load shader into shader manager and retrieve it
  shapeShader = this->shaderManager->loadShader("../res/shaders/shape3D.vert",
                                                "../res/shaders/shape3D.frag",
                                                nullptr, "shape");

  // Configure text shader and renderer
  textShader = shaderManager->loadShader(
      "../res/shaders/text.vert", "../res/shaders/text.frag", nullptr, "text");
}

void Engine::initShapes() {
  cube =
      make_unique<Cube>(shapeShader, vec3(0.0f, 0.0f, 0.0f),
                        vec3(0.5f, 0.5f, 0.5f), color(1.0f, 0.0f, 0.0f, 1.0f));
}

void Engine::initMatrices() {
  // The view matrix is the camera's position and orientation in the world
  // We start at (0, 0, 3) and look at (0, 0, 0) with the up vector being (0, 1,
  // 0)
  view = lookAt(vec3(0.0f, 0.0f, 3.0f), vec3(0.0f, 0.0f, 0.0f),
                vec3(0.0f, 1.0f, 0.0f));
  // The projection matrix for 3D is distorted by 45 degrees to give a
  // perspective view
  projection = glm::perspective(
      glm::radians(45.0f),
      static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
  model = glm::mat4(1.0f);
}

void Engine::processInput() {
  glfwPollEvents();

  // Set keys to true if pressed, false if released
  for (int key = 0; key < 1024; ++key) {
    if (glfwGetKey(window, key) == GLFW_PRESS)
      keys[key] = true;
    else if (glfwGetKey(window, key) == GLFW_RELEASE)
      keys[key] = false;
  }

  // Close window if escape key is pressed
  if (keys[GLFW_KEY_ESCAPE])
    glfwSetWindowShouldClose(window, true);

  // Rotating the cube up rotates around the x-axis, so we update rotateX
  if (keys[GLFW_KEY_UP])
      rotateX += 0.1f;
  // TODO: Complete rotation with other three arrow keys
    if (keys[GLFW_KEY_DOWN])
        rotateX -= 0.1f;
    if (keys[GLFW_KEY_RIGHT])
      rotateY += 0.1f;
    if (keys[GLFW_KEY_LEFT])
        rotateY -= 0.1f;

  // TODO: When the user presses g, move the camera forward
  //       to make the cube appear bigger
    if (keys[GLFW_KEY_G])
        cameraZ += 0.1f;
  // TODO: When the user presses s, move the camera backward
  //       to make the cube appear smaller
    if (keys[GLFW_KEY_S])
        cameraZ -= 0.1f;
}

void Engine::update() {
  // Calculate delta time
  float currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  // Resetting model and view matrices every frame prevents the cube from
  // spinning
  model = glm::mat4(1.0f);
  view = glm::mat4(1.0f);


}

void Engine::render() {
  // Clear the screen before rendering the frame
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color
  glClear(GL_COLOR_BUFFER_BIT |
          GL_DEPTH_BUFFER_BIT); // Also need to clear the depth buffer bit

  // Rotate cube if arrow keys are pressed
  model = glm::rotate(model, rotateX, glm::vec3(1.0f, 0.0f, 0.0f));
  // TODO: rotate the model matrix around the y axis
    model = glm::rotate(model, rotateY, glm::vec3(0.0f, 0.0f, 1.0f));
  // Move the camera back 3 units to view the cube (otherwise we would be inside
  // it)
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, cameraZ));

  // Draw cube
  shapeShader.use();
  cube->setUniforms(model, view, projection);
  cube->draw(model, view, projection);

  glfwSwapBuffers(window);
}

bool Engine::shouldClose() { return glfwWindowShouldClose(window); }
