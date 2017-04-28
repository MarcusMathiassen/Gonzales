#include "MM.h"
#include <stdio.h>
int main()
{

  //   for (u8 y = 0; y < 16; ++y)
  //   {
  //     for (u8 x = 0; x < 16; ++x)
  //     {
  //       printf("vec2(%f, %f)\n", x*0.0625f, y*0.0625f);
  //     }
  //   }

  for (int k = 0; k < 4; ++k)
  {
    float i = 0.0625f;
    float x = i * (k / 2);
    float y = i * (k*k % 3);

    printf("x: %f y: %f\n", x, y);
  }

	Engine engine;
  engine.vsync = 0;
	engine.init();

  engine.textManager = new TextManager();

  Text text;
  text.txt = "Hello Can YOU SEE 243565374463512";
  text.x = 0;
  text.y = 0;
  engine.addText(text);

	engine.start();
}
