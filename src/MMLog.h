#pragma once

#include "MM.h"
#include <cstdio>
#include <cstdint>

static void MMLog(uint8_t TYPE, const char *text)
{
  switch (TYPE)
  {
    case MM_NOTE:  printf("MM_NOTE:  %s\n", text); break;
    case MM_ERROR: printf("MM_ERROR: %s\n", text); break;
  }
}