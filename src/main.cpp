#include "MM.h"
#include <stdio.h>
int main()
{
  Engine engine;
  engine.width = 512;
  engine.height = 512;
  engine.vsync = 0;
  engine.init();

  // GameObject cube(
  //   "./res/cubeUV",
  //   "./res/marble.jpg",
  //   "./res/MM_BasicGameObject.vs",
  //   "./res/MM_BasicGameObject.fs");
 // engine.addGameObject("cube_01", cube);

  Text text;
  text.pos.x = -1;
  text.pos.y = -0.9375;
  text.str = "!#¤780+-.,æ'¨øåPAÆÅØ^Ålø";
  u32 id = engine.addText(text);

	engine.start();
}
