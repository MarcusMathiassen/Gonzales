#include "MM.h"
#include <stdio.h>
int main()
{
	Engine engine;
  engine.vsync = 0;
	engine.init();

  engine.textManager = new TextManager();

  Text text;
  text.txt = "M A R C U S";
  text.x = 0;
  text.y = 0;
  engine.addText(text);

	engine.start();
}
