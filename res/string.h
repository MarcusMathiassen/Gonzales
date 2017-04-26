#pragma once

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;

typedef int8_t      s8;
typedef int16_t     s16;
typedef int32_t     s32;
typedef int64_t     s64;

typedef float       f32;
typedef double      f64;


struct string
{
  char*              data    { NULL  };
  u16                length  { 0 };

  string() = default;
  ~string() { delete data; }

  const char* c_str() { return data; }
  char operator [] (u16 i)
  {
    return data[i];
  }

  string(const char* txt)
  {
    length = strlen(txt);
    data = (char*)malloc(length+1);
    strcpy(data, txt);
  }
  const char* operator + (const string& s)
  {
    length += s.length;
    data = (char*)realloc(data, length+1);
    strcat(data, s.data);
    return data;
  }

  const char* operator + (const char* txt)
  {
    length += strlen(txt);
    data = (char*)realloc(data, length+1);
    strcat(data, txt);
    return data;
  }

  void operator += (const string& s)
  {
    length += s.length;
    data = (char*)realloc(data, length+1);
    strcat(data, s.data);
  }
  void operator = (const string& s)
  {
    length = s.length;
    data = (char*)malloc(length+1);
    strcpy(data, s.data);
  }

  void operator += (const char* txt)
  {
    length += strlen(txt);
    data = (char*)realloc(data, length+1);
    strcat(data, txt);
  }
  void operator = (const char* txt)
  {
    length = strlen(txt);
    data = (char*)malloc(length+1);
    strcpy(data, txt);
  }
};

static const char* strrev(char* buffer, u16 length)
{
  char *p1 = buffer;
  char *p2 = buffer + length-1;
  while (p1 < p2)
  {
    char tmp = *p1;
    *p1++ = *p2;
    *p2-- = tmp;
  }
  return buffer;
}

static string itos(const u32 i)
{
  static char buffer[8]{0};
  u32 res{i};
  u8 index{0};
  while (res)
  {
    u32 rem=res%10;
    res *= 0.1f;
    buffer[index++]='0'+rem;
  }
  return string(strrev(buffer, index));
}

static const char* itoa(const u32 i)
{
  static char buffer[8]{0};
  u32 res{i};
  u8 index{0};
  while (res)
  {
    u32 rem=res%10;
    res *= 0.1f;
    buffer[index++]='0'+rem;
  }
  return strrev(buffer, index);
}

static const char* ftoa(const f32 f)
{
  static char integers[9]{0};
  s32 integer_part{(s32)f};
  u8 index_i{0};
  bool isNegative{false};
  if (integer_part < 0)
  {
    isNegative = true;
    integer_part *= -1;
  }
  u32 res{(u32)integer_part};

  integers[index_i++] = '.';
  while (res)
  {
    u32 rem=res%10;
    res *= 0.1f;
    integers[index_i++]='0'+rem;
  }
  if (isNegative) integers[index_i++] = '-';
  strrev(integers, index_i);

  static char decimals[6]{0};
  f32 float_part{ f - (f32)integer_part };
  float_part *= 1e6f;
  float_part += 1; // offset
  res = (u32)float_part;
  u8 index_f{0};
  while (res)
  {
    u32 rem=res%10;
    res *= 0.1f;
    decimals[index_f++]='0'+rem;
  }
  strrev(decimals, index_f);

  static char buffer[15]{0};
  memmove(buffer, integers, index_i);
  memmove(buffer+index_i, decimals, index_f);
  buffer[index_f] = '\0';
  return buffer;
}

static string ftos(const f32 f)
{
  static char integers[9]{0};
  s32 integer_part{(s32)f};
  u8 index_i{0};
  bool isNegative{false};
  if (integer_part < 0)
  {
    isNegative = true;
    integer_part *= -1;
  }
  u32 res{(u32)integer_part};

  integers[index_i++] = '.';
  while (res)
  {
    u32 rem=res%10;
    res *= 0.1f;
    integers[index_i++]='0'+rem;
  }
  if (isNegative) integers[index_i++] = '-';
  strrev(integers, index_i);

  static char decimals[6]{0};
  f32 float_part{ f - (f32)integer_part };
  float_part *= 1e6f;
  float_part += 1; // offset
  res = (u32)float_part;
  u8 index_f{0};
  while (res)
  {
    u32 rem=res%10;
    res *= 0.1f;
    decimals[index_f++]='0'+rem;
  }
  strrev(decimals, index_f);

  static char buffer[15]{0};
  memmove(buffer, integers, index_i);
  memmove(buffer+index_i, decimals, index_f);
  buffer[index_f] = '\0';
  return string(buffer);
}
