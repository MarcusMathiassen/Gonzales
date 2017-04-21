
#include "MM.h"

TearingTest *testTearing;

int main()
{
  MMApp app;
  app.openGLVersion = 4.1f;
  mmInit(app);

  testTearing = new TearingTest();

  mmStart(app);

  delete testTearing;
}

void draw()
{
  testTearing->draw();
  mmDrawText("Hello from mmDrawText!", 0, 0);
}
