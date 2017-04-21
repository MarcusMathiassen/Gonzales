
#include "MM.h"

int main()
{
  MMApp app;
  mmInit(app);
  mmStart(app);
}

void draw()
{
  mmDrawText("Hello from mmDrawText!", 0, 0);
}
