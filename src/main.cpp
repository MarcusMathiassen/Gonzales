#include "MM.h"
#include <stdio.h>
int main()
{
  Engine engine;
  engine.width = 1024;
  engine.height = 1024;
  engine.vsync = 0;
  engine.init();

  Text text;
  text.x = -1;
  text.y = -0.9375;
  text.str = "!#¤780+-.,æ'¨øåPAÆÅØ^Ålø";
  u32 id = engine.addText(text);

	engine.start();
}
