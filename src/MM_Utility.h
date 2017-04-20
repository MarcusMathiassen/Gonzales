#ifndef _MM_UTILITY_H_
#define _MM_UTILITY_H_

#include "MM_Log.h"
#include <cstdlib>
#include <cstdio>

#include <glm/glm.hpp>

/* Declarations */
static bool MM_readFile(const char *file, char **buffer);
static glm::vec3 MM_HSVtoRGB(float h, float s, float v);


/* Definitions */
static bool MM_readFile(const char *file, char **buffer)
{
  if (*buffer != NULL)
  {
    MM_log(MM_ERROR, "MM_readFile: input buffer must be NULL");
    return false;
  }

  FILE *fp;
  fp = fopen(file,"rb");
  if (fp != NULL)
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

static glm::vec3 MM_HSVtoRGB(float h, float s, float v) {
  float p, q, t, f;
  int  i;
  float r,g,b;

  h = (h >= 360.0) ? 0 : h;
  h /= 60.0;
  i = (int)h;
  f = h - i;
  p = v * (1.0 - s);
  q = v * (1.0 - (s * f));
  t = v * (1.0 - (s * (1.0 - f)));

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
