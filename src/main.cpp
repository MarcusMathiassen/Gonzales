
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

  std::stringstream ss;
  ss << app.currentFPS << "fps " << (float)app.deltaTime << "ms";
  sprintf (buffer, "%dfps %0.3fms", app.currentFPS, (float)app.deltaTime);
  mmDrawText(ss.str(), -1.0f, 0.9f);
}
