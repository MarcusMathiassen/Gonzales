#include "MM.h"
#include <stdio.h>
int main()
{
  Engine engine;
  engine.vsync = 0;
  engine.init();

  Text text;
  text.x = -1;
  text.y = -0.8;
  text.str = "fps";
  text.color = glm::vec4(getHSV(255,1.0f,1.0f),1.0);
  u32 id = engine.addText(text);
  engine.fps_info = id;

	engine.start();
}
