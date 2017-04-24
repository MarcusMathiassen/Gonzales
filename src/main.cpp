#include "MM.h"

#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>
#include <cmath>

MMApp app;
MMGameObject *cube;
MMButton     *button;
int main()
{
  app.openGLVersion = 4.1f;
  app.width = 512;
  app.height = 512;
  app.vsync = 0;
  mmInit(app);

  cube = new MMGameObject("./res/cubeUV.obj",
                          "./res/cubeUV_num.png",
                          "./res/MM_BasicGameObject.vs",
                          "./res/MM_BasicGameObject.fs");

  button = new MMButton(-1,-1,0.5,0.1,"./res/marble.jpg");

  mmStart(app);
  delete cube;
}

char buffer[50];
void draw()
{
  cube->transform.rot.z = glfwGetTime()*0.5;
  cube->transform.rot.y = glfwGetTime()*0.3;
  cube->transform.rot.x = glfwGetTime();

  cube->draw();
  button->draw();

  mmDrawText("Hello, this is a string literal", -1.0f, 0.9f);
  sprintf (buffer, "Hello, this is a char[]: %dfps %0.3fms", app.currentFPS, (float)app.deltaTime);
  mmDrawText(buffer, -1.0f, 0.8f);
  std::stringstream ss;
  ss << "Hello, this is a std::string: " << app.currentFPS << "fps " << (float)app.deltaTime << "ms";
  mmDrawText(ss.str(), -1.0f, 0.7f);
}
