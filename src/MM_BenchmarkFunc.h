#ifndef _MM_BENCHMARKFUNC_H_
#define _MM_BENCHMARKFUNC_H_

#define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <cstdint>
#include <cstdio>

static void  MM_benchmarkFuncs()
{
  double timeStart{glfwGetTime()};
  for (int i = 0; i < 1e7; ++i)
  {

  }
  double newFuncTime{glfwGetTime()-timeStart};

  timeStart = glfwGetTime();
  for (int i = 0; i < 1e7; ++i)
  {

  }
  double oldFuncTime{glfwGetTime()-timeStart};



  printf("NewFunc used: %fms\n", newFuncTime*1e3);
  printf("OldFunc used: %fms\n", oldFuncTime*1e3);

  uint8_t fastest = (newFuncTime < oldFuncTime) ? 1 : 0;
  double diff;
  switch (fastest)
  {
    case 1:
          diff = (newFuncTime/oldFuncTime) * 100.0;
          printf("NewFunc faster by %f%%\n",100.0-diff);
          break;
    case 0:
          diff = (oldFuncTime/newFuncTime) * 100.0;
          printf("OldFunc faster by %f%%\n",100.0-diff);
          break;
  }
}

#endif
