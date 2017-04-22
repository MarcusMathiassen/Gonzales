
#include "MM.h"

#include <cstring>

TearingTest *test;
MMApp app;

int main()
{
  app.openGLVersion = 4.1f;
  app.width = 1024;
  app.height = 1024;
  mmInit(app);

  test = new TearingTest;

  mmStart(app);
  delete test;
}

char buffer[50];
void draw()
{
  test->draw();

  mmDrawText("@Marcus Mathiassen", -1, -1);

  sprintf (buffer, "%dfps %0.3fms", app.currentFPS, (float)app.deltaTime);
  mmDrawText(buffer, -1.0f, 0.9f);
}
