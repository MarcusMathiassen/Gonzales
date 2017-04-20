#ifndef _MM_UTILITY_H_
#define _MM_UTILITY_H_

#include "MM_Log.h"
#include <cstdlib>
#include <cstdio>

#include <glm/glm.hpp>

/* Definitions */
static bool MM_readFile(const char *file, char **buffer)
{
  if (*buffer != NULL)
  {
    MM_log(MM_ERROR, "MM_readFile: input buffer must be NULL");
    return false;
  }

  FILE *fp;
  if (fopen_s(&fp, file, "rb") == 0)
  {
    if (fseek(fp, 0L, SEEK_END) == 0)
    {
      long buffer_size = ftell(fp);

      *buffer = (char*)calloc(buffer_size, sizeof(char));

      if (fseek(fp, 0L, SEEK_SET) != 0)
        MM_log(MM_ERROR, "MM_readFile: error allocating buffer");

      fread(*buffer, sizeof(char), buffer_size, fp);
      if (ferror(fp) != 0)
        MM_log(MM_ERROR, "MM_readFile: error reading file");
    }
    fclose(fp);
  }
  else
  {
    MM_log(MM_ERROR, "MM_readFile: error reading file.");
    return false;
  }

  return true;
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

  float r{0}, g{0}, b{0};

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
