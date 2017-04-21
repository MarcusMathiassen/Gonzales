
#include "MM.h"

TearingTest *test;

int main()
{
  MMApp app;
  app.openGLVersion = 4.1f;
  mmInit(app);

  test = new TearingTest();
  
  mmStart(app);

  delete test;
}

void draw()
{
  //test->draw();
  mmDrawText();
}
