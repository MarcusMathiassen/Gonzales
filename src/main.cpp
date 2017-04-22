
#include "MM.h"

#include <cstring>

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

void draw()
{
  test->draw();
  char buffer[50];
  sprintf (buffer, "%dfps %0.3fms", app.currentFPS, (float)app.deltaTime);
  mmDrawText(buffer, -1, 0.9);
}
