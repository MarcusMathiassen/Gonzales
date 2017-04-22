
#include "MM.h"

#include <cstring>
#include <string>
#include <sstream>

TearingTest *test;
MMApp app;

int main()
{
  app.openGLVersion = 4.1f;
  app.width = 512;
  app.height = 512;
  mmInit(app);

  test = new TearingTest;

  mmStart(app);
  delete test;
}

char buffer[50];
void draw()
{
  test->draw();

  mmDrawText(3, -1.0f, 0.8f);
  mmDrawText(3.0, -1.0f, 0.7f);
  mmDrawText(3.0f, -1.0f, 0.6f);
  mmDrawText(app.currentFPS, -1.0f, 0.5f);
  mmDrawText(app.deltaTime, -1.0f, 0.4f);
  mmDrawText("Hello, this is a string literal", -1.0f, 0.3f);
  sprintf (buffer, "Hello, this is a char[]: %dfps %0.3fms", app.currentFPS, (float)app.deltaTime);
  mmDrawText(buffer, -1.0f, 0.2f);
  std::stringstream ss;
  ss << "Hello, this is a std::string: " << app.currentFPS << "fps " << (float)app.deltaTime << "ms";
  mmDrawText(ss.str(), -1.0f, 0.1f);
}
