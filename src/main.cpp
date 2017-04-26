#include "MM.h"

#include <cstdio>

int main()
{
//   struct rf{ float x;float y; } rf[256];
//   for (u8 y = 0; y < 16; ++y)
//       for (u8 x = 0; x < 16; ++x)
//       {
//         rf[16 * y+x].x = x*0.0625;
//         rf[16 * y+x].y = y*0.0625;
//       }

//   for (u32 i = 0; i < 256; ++i)
//         printf(
//           "vec2(%f, %f)\n"
//           "vec2(%f, %f)\n"
//           "vec2(%f, %f)\n"
//           "vec2(%f, %f)\n",
//           rf[i].x,          rf[i].y,
//           rf[i].x,          rf[i].y + 0.0625f,
//           rf[i].x + 0.0625f, rf[i].y + 0.0625f,
//           rf[i].x + 0.0625f, rf[i].y);
// return 0;
	Engine engine;
  engine.vsync = 0;
	engine.init();

  GameObject cube("./res/sphereUV.obj",
                  "./res/earth.jpg",
                  "./res/MM_BasicGameObject.vs",
                  "./res/MM_BasicGameObject.fs");

  engine.textManager = new TextManager();

  Text text;
  text.txt = "Hello this is a test";
  text.x = 0;
  text.y = 0;
  engine.addText(text);

  engine.addGameObject(cube);

	engine.start();
}
