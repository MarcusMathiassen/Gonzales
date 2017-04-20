#include "MM_App.h"

bool MM_App::running()
{
  if (glfwWindowShouldClose(window))
    return false;

  if (!hasRan)
  {
    glfwSetTime(0.0);
    hasRan = true;
  }

  double timeStartFrame{ glfwGetTime() };
  glfwPollEvents();

  double timeRemainingSec{ (glfwGetTime() - timeStartFrame)};
  double timeRemainingMS{ timeRemainingSec * 1000.0 };
  while (timeRemainingSec < fixedFrameTime)
  {

    /* If we can sleep, do so. Depends on sleep resolution */
    if (timeRemainingSec > 3.00)
    {
      #ifdef _WIN32
      //Sleep(timeRemainingMS);
      //std::cout << "sleeping for " << timeRemainingMS*1000.0f << "ms\n";
      //else
      /* UNIX sleep*/
      #endif
    }

    /* Else burn CPU cycles */
    timeRemainingSec = (glfwGetTime() - timeStartFrame)*1000.0;
  }

  deltaTime = (float)((glfwGetTime() - timeStartFrame)*1000.0f);
  printf("%f\n", deltaTime);

  glfwSwapBuffers(window);
  return true;
}

void MM_App::shutdown()
{
  glfwTerminate();
}