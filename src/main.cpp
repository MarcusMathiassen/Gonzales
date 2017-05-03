#include "MM.h"
#include <stdio.h>

int main()
{
  Engine engine;
  engine.width = 512;
  engine.height = 512;
  engine.init();

  GameObject cube("./res/sphereU","./res/earth","./res/MM_BasicGameObject.vs","./res/MM_BasicGameObject.fs");
  engine.addGameObject("cube_01", cube);

	engine.start();
}

