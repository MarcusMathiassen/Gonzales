#pragma once

#include "MM_Typedefs.h"

struct Resource
{
  /* Resource name */
  string name;
  string type;


  /* file info */
  u64    size;
  string file_name;
  string file_path;
};
