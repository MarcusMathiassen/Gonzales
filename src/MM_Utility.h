#pragma once

#include "MM_Typedefs.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <iostream>

#include <vector>
#include <sstream>
#include <cmath>

#include <glm/glm.hpp>

#ifdef _WIN32
  #include <windows.h>
#elif __APPLE__
  #include <ctime>
  #include <unistd.h>
#endif

static void loadOBJ(const char* file, vector<vec3> *vertices, vector<vec3> *normals, vector<vec2> *uvs, vector<u16>  *indices);
static void limitFPS(u32 framesPerSecond, f64 timeStartFrame);
static void readFile(const char *file, char **buffer);
#ifdef _WIN32
  static vec3 getHSV(u16 h, f32 s, f32 v);
#else
  static constexpr vec3 getHSV(u16 h, f32 s, f32 v);
#endif
static void sleepForSec(f32 sec);
static void sleepForMs(f32 ms);

static void sleepForMs(f32 ms)
{
  #ifdef _WIN32
    Sleep((DWORD)ms);
  #elif __APPLE__
    nanosleep((const struct timespec[]){{0, static_cast<long>(ms*1e6)}}, NULL);
  #endif
}

static void sleepForSec(f32 sec)
{
  #ifdef _WIN32
    Sleep((DWORD)(1000.0/sec));
  #elif __APPLE__
    nanosleep((const struct timespec[]){{0, static_cast<long>((1000.0/sec)*1e6)}}, NULL);
  #endif
}

static void limitFPS(u32 framesPerSecond, f64 timeStartFrame)
{
  const f64 frametime = (f64)(1000.0/framesPerSecond);
  f64 timeSpentFrame{(glfwGetTime() - timeStartFrame) * 1000.0};
  #ifdef _WIN32
    #define SLEEP_TIME_OFFSET 1.0
  #elif __APPLE__
    #define SLEEP_TIME_OFFSET 2.0
  #endif
  const f64 sleepTime{(frametime-SLEEP_TIME_OFFSET) - timeSpentFrame};
  if (sleepTime > 0)
  {
  #ifdef _WIN32
    Sleep((DWORD)sleepTime);
  #elif __APPLE__ // @TODO: fix unix sleep. Very unstable compared to WIN32
    nanosleep((const struct timespec[]){{0, static_cast<long>(sleepTime*1e6)}}, NULL);
  #endif
    while (timeSpentFrame < frametime)
    {
      timeSpentFrame = (glfwGetTime() - timeStartFrame) * 1000.0;
    }
  }
}

static void readFile(const char* file, char** buffer)
{
  string buff, line;
  std::ifstream fileIn(file);
  while (std::getline(fileIn, line)) buff += line+'\n';
  *buffer = (char*)malloc((buff.length()+1) * sizeof(char));
  strcpy(*buffer, buff.c_str());
}

#ifdef _WIN32
  static vec3 getHSV(u16 h, f32 s, f32 v)
#else
  static constexpr vec3 getHSV(u16 h, f32 s, f32 v)
#endif
{
  h = (h >= 360) ? 0 : h;
  const f32 hue { (f32)h * 0.016666f };

  const u8 i    { (u8)hue };
  const f32 f   { hue - i };
  const f32 p   { v * (1.0f - s) };
  const f32 q   { v * (1.0f - s*f) };
  const f32 t   { v * (1.0f - s*( 1.0f-f )) };

  f32 r{0.0f}, g{0.0f}, b{0.0f};

  switch(i)
  {
    case 0: r = v; g = t; b = p; break;
    case 1: r = q; g = v; b = p; break;
    case 2: r = p; g = v; b = t; break;
    case 3: r = p; g = q; b = v; break;
    case 4: r = t; g = p; b = v; break;
    case 5:
    default: r = v; g = p; b = q; break;
  }
  return vec3(r,g,b);
}

static void loadOBJ(
	const char* file,
	vector<vec3> *vertices,
	vector<vec3> *normals,
	vector<vec2> *uvs,
	vector<u16>  *indices)
{
	char *buffer;
	readFile(file, &buffer);

	std::stringstream ss;
	ss << buffer;

	if (NULL != buffer)
		free(buffer);

	vector<vec3> temp_vertices;
	vector<vec3> temp_normals;
	vector<vec2> temp_uvs;
	vector<u16> vert_indices, uv_indices, norm_indices;

	bool hasUV{ false }, hasNormal{ false };

	string line;
	while (std::getline(ss, line))
	{
		if (line[0] == '#') continue;

		if (line[0] == 'v')
		{
			switch (line[1])
			{
			case ' ':
			{
				vec3 v;
				sscanf(line.c_str() + 1, "%f %f %f", &v.x, &v.y, &v.z);
				temp_vertices.emplace_back(v);
			}
			break;

			case 'n':
			{
				hasNormal = true;
				vec3 n;
				sscanf(line.c_str() + 2, "%f %f %f", &n.x, &n.y, &n.z);
				temp_normals.emplace_back(n);
			}
			break;

			case 't':
			{
				hasUV = true;
				vec2 t;
				sscanf(line.c_str() + 2, "%f %f", &t.x, &t.y);
				temp_uvs.emplace_back(t);
			}
			break;
			}
		}
		else if (line[0] == 'f')
		{
			u16 v_i[3]{ 0 }, vn_i[3]{ 0 }, vt_i[3]{ 0 };

			u8 numOfSlashes{ 0 };
			for (const auto& chr : line)
				if (chr == '/') ++numOfSlashes;
			switch (numOfSlashes)
			{
			case 0:
			{
				sscanf(line.c_str() + 2, "%hd %hd %hd",
					&v_i[0],
					&v_i[1],
					&v_i[2]);
			} break;

			case 3:
			{
				sscanf(line.c_str() + 2, "%hd/%hd %hd/%hd %hd/%hd",
					&v_i[0], &vt_i[0],
					&v_i[1], &vt_i[1],
					&v_i[2], &vt_i[2]);
			} break;

			case 6:
			{
				if (hasNormal && hasUV)
				{
					sscanf(line.c_str() + 2, "%hd/%hd/%hd %hd/%hd/%hd %hd/%hd/%hd",
						&v_i[0], &vt_i[0], &vn_i[0],
						&v_i[1], &vt_i[1], &vn_i[1],
						&v_i[2], &vt_i[2], &vn_i[2]);
				}
				else
				{
					sscanf(line.c_str() + 2, "%hd//%hd %hd//%hd %hd//%hd",
						&v_i[0], &vn_i[0],
						&v_i[1], &vn_i[1],
						&v_i[2], &vn_i[2]);
				}
			} break;

			case 9:
			{
				sscanf(line.c_str() + 2, "%hd//%hd//%hd %hd//%hd//%hd %hd//%hd//%hd",
					&v_i[0], &vt_i[0], &vn_i[0],
					&v_i[1], &vt_i[1], &vn_i[1],
					&v_i[2], &vt_i[2], &vn_i[2]);
			} break;
			}

			vert_indices.emplace_back(v_i[0]);
			vert_indices.emplace_back(v_i[1]);
			vert_indices.emplace_back(v_i[2]);
			if (hasUV)
			{
				uv_indices.emplace_back(vt_i[0]);
				uv_indices.emplace_back(vt_i[1]);
				uv_indices.emplace_back(vt_i[2]);
			}
			if (hasNormal)
			{
				norm_indices.emplace_back(vn_i[0]);
				norm_indices.emplace_back(vn_i[1]);
				norm_indices.emplace_back(vn_i[2]);
			}
		}
	}

	for (const auto& vert_i : vert_indices)
		vertices->emplace_back(temp_vertices[vert_i - 1]);

	if (hasNormal)
	{
		for (const auto& norm_i : norm_indices)
			normals->emplace_back(temp_normals[norm_i - 1]);
	}

	if (hasUV)
	{
		for (const auto& uv_i : uv_indices)
			uvs->emplace_back(temp_uvs[uv_i - 1]);
	}

  std::cout << "file: " << file << '\n';
  std::cout << "      positions: " << vertices->size() << '\n';
  std::cout << "      normals: " << normals->size() << '\n';
  std::cout << "      uvs: " << uvs->size() << '\n';

}
