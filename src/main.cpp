#include "MM.h"

#include <iostream>

int main()
{
	Engine engine;
	engine.init();

  Mesh mesh;
  Texture texture;
  Transform transform;
  GameObject gameobject;

  //GameObject cube("./res/cube.obj","./res/marble.jpg","./res/MM_BasicGameObject.vs","./res/MM_BasicGameObject.fs");
  //GameObject cube;
  //cube.mesh = Mesh("./res/cube.obj");
  //engine.addGameObject(cube);

  UIButton button(0, 0, 0.2, 0.1);
  button.texture = Texture("./res/marble.jpg", GL_LINEAR);
  button.text.txt = "HELLO";
  engine.addUI(button);

	engine.start();
}
