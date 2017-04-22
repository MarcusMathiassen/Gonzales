
#include "MM.h"

TearingTest *test;
MMApp app;

int main()
{
  app.openGLVersion = 4.1f;
  app.width = 512;
  app.height = 512;
  app.vsync = false;
  app.framerate = 0;
  mmInit(app);

  test = new TearingTest;

  mmStart(app);

  delete test;
}

void draw()
{
  //test->draw();
  mmDrawText(std::to_string(app.currentFPS).c_str(),-1, 0.9);
}
