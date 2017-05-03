#pragma once

#include "MM_Typedefs.h"

static u64 get_cpu_freq() {
  u64 num = 0;
  size_t size = sizeof(num);

  if (sysctlbyname("hw.cpufrequency", &num, &size, NULL, 0) < 0) {
    perror("sysctl");
  }
  return num;
}

static u64 get_cpu_cores() {
  u64 num = 0;
  size_t size = sizeof(num);

  if (sysctlbyname("hw.physicalcpu_max", &num, &size, NULL, 0) < 0) {
    perror("sysctl");
  }
  return num;
}

static u64 get_cpu_threads() {
  u64 num = 0;
  size_t size = sizeof(num);

  if (sysctlbyname("hw.logicalcpu_max", &num, &size, NULL, 0) < 0) {
    perror("sysctl");
  }
  return num;
}

static char* get_cpu_brand() {
  char buffer[128];
  size_t bufferlen = 128;

  sysctlbyname("machdep.cpu.brand_string", &buffer, &bufferlen, NULL, 0);

  return buffer;
}
