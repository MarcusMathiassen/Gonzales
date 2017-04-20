#include "MM_App.h"
#include "MM_Utility.h"

#ifdef _WIN32
  //#include "window.h"
#elif __APPLE__
  #include <ctime>
  #include <unistd.h>
#endif

MM_App::~MM_App()
{
  glfwTerminate();
}

// Call this from a
void MM::App::pollEvents()
{
  glfwSwapBuffers(window);
  // sleep for 16.666ms
  nanosleep((const struct timespec[]){{0, static_cast<long>(16.666*1e6)}}, NULL);
}

bool MM_App::running()
{
  double timeStartFrame{ glfwGetTime() };
  glfwPollEvents();

  fixedFrametime = 1000.0f/fixedFramerate;

  if (timeStartFrame - timeSinceStart >= 1.0) {
    printf("%dfps %0.03fms\n", currentFPS, deltaTime);
    ++timeSinceStart;
  }

  if (glfwWindowShouldClose(window))
    return false;

  MM_limitFPS(fixedFramerate, timeStartFrame);

  deltaTime = (float)((glfwGetTime() - timeStartFrame)*1000.0f);
  currentFPS = (uint32_t)(1.0/(deltaTime*0.001)+0.09);

  glfwSwapBuffers(window);
  return true;
}
