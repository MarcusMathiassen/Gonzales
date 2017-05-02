#include "MM.h"
#include <stdio.h>
int main()
{
  Engine engine;
  engine.width = 512;
  engine.height = 512;
  engine.init();

  GameObject cube("./res/cubeUV","./res/marble.jpg","./res/MM_BasicGameObject.vs","./res/MM_BasicGameObject.fs");
  engine.addGameObject("cube_01", cube);

  Text text;
  text.color = glm::vec4(1,0,0,1);
  text.pos.x = -1;
  text.pos.y = -0.9375;
  u32 id = engine.addText(text);

	engine.start();
}
