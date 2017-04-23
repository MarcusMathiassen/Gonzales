
#include "MM.h"

#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>
#include <cmath>

MMApp app;
MMGameObject *icosphere;
int main()
{
  app.openGLVersion = 4.1f;
  app.width = 1024;
  app.height = 1024;
  mmInit(app);

  icosphere = new MMGameObject( "./res/cube.obj",
                                "./res/MM_BasicGameObject.vs",
                                "./res/MM_BasicGameObject.fs");
  mmStart(app);
  delete icosphere;
}

char buffer[50];
void draw()
{
  icosphere->transform.rot.z = glfwGetTime()*0.5;
  icosphere->transform.rot.y = glfwGetTime()*0.1;
  icosphere->transform.rot.x = glfwGetTime();
  icosphere->transform.pos.z = sin(glfwGetTime());

  icosphere->draw();

  mmDrawText("Hello, this is a string literal", -1.0f, 0.9f);
  sprintf (buffer, "Hello, this is a char[]: %dfps %0.3fms", app.currentFPS, (float)app.deltaTime);
  mmDrawText(buffer, -1.0f, 0.8f);
  std::stringstream ss;
  ss << "Hello, this is a std::string: " << app.currentFPS << "fps " << (float)app.deltaTime << "ms";
  mmDrawText(ss.str(), -1.0f, 0.7f);
}
