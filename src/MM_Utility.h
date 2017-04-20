#ifndef _MM_UTILITY_H_
#define _MM_UTILITY_H_

#define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <cstdio>
#include <cstdint>

#include <glm/glm.hpp>

#ifdef _WIN32
  //#include "window.h"
#elif __APPLE__
  #include <ctime>
  #include <unistd.h>
#endif


/* Declarations */
static void MM_limitFPS(uint32_t framesPerSecond, double timeStartFrame);
static void MM_readFile(const char *file, char **buffer);
static glm::vec3 MM_HSVtoRGB(uint16_t h, float s, float v);


/* Definitions */
static void MM_limitFPS(uint32_t framesPerSecond, double timeStartFrame)
{
  double frametime = (double)(1000.0/framesPerSecond);

  // Frame limiter
  double timeSpentFrame{(glfwGetTime() - timeStartFrame) * 1000.0};
  double sleepTime{(frametime-1.1) - timeSpentFrame};
  if (sleepTime > 0)
  {
  #ifdef _WIN32
    //Sleep(timeRemainingMS);
  #elif __APPLE__
    nanosleep((const struct timespec[]){{0, static_cast<long>(sleepTime*1e6)}}, NULL);
  #endif
    while (timeSpentFrame < frametime)
    {
      /* Spinlock the leftovers */
      timeSpentFrame = (glfwGetTime() - timeStartFrame) * 1000.0;
    }
  }
}

static void MM_readFile(const char *file, char **buffer)
{
  if (*buffer != NULL)
    return;

  FILE *fp;
  #ifdef _WIN32
  if (fopen_s(&fp, file, "rb") == 0)
  #else
    fp = fopen(file, "rb");
    if (fp != NULL)
  #endif
  {
    if (fseek(fp, 0L, SEEK_END) == 0)
    {
      long buffer_size = ftell(fp);
      *buffer = (char*)calloc(buffer_size, sizeof(char));
      fseek(fp, 0L, SEEK_SET);
      fread(*buffer, sizeof(char), buffer_size, fp);
    }
    fclose(fp);
  }
}

static glm::vec3 MM_HSVtoRGB(uint16_t h, float s, float v)
{
  h = (h >= 360) ? 0 : h;
  const float hue{ (float)h * 0.016666f };

  const uint8_t i { (uint8_t)hue };
  const float f   { hue - i };
  const float p   { v * (1.0f - s) };
  const float q   { v * (1.0f - s*f) };
  const float t   { v * (1.0f - s*(1.0f-f)) };

  float r{0.0f}, g{0.0f}, b{0.0f};

  switch(i) {
    case 0: r = v; g = t; b = p; break;
    case 1: r = q; g = v; b = p; break;
    case 2: r = p; g = v; b = t; break;
    case 3: r = p; g = q; b = v; break;
    case 4: r = t; g = p; b = v; break;
    case 5:
    default: r = v; g = p; b = q; break;
  }
  return glm::vec3(r,g,b);
}

#endif
