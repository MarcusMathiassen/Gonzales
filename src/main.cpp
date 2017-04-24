#include "MM.h"

#include <iostream>

int main()
{
	Engine engine;
  engine.vsync = 0;
	engine.init();

  GameObject cube("./res/cubeUV_2.obj", "./res/cubeUV_num_2.png", "./res/MM_BasicGameObject.vs", "./res/MM_BasicGameObject.fs");
  engine.addGameObject(cube);

  UIButton button(-1.0f, -0.9f, 0.5f, 0.2f, "./res/marble.jpg");
  engine.addUI(button);

	engine.start();
}
