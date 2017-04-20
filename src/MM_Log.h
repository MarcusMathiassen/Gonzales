#ifndef _MM_LOG_H
#define _MM_LOG_H

#include "MM.h"
#include <cstdio>
#include <cstdint>

static void MM_log(uint8_t TYPE, const char *text)
{
  switch (TYPE)
  {
    case MM_NOTE:  printf("MM_NOTE:  %s\n", text); break;
    case MM_ERROR: printf("MM_ERROR: %s\n", text); break;
  }
}

#endif
