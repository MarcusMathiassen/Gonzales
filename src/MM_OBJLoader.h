#ifndef _MM_OBJLOADER_H_
#define _MM_OBJLOADER_H_

#include "MM_Utility.h"
#include <glm/glm.hpp>
#include <cstdio>
#include <string>
#include <cstdint>
#include <vector>
#include <sstream>
#include <cmath>

static bool isSimilarVec2(const glm::vec2 &v1, const glm::vec2 &v2);
static bool isSimilarVec3(const glm::vec3 &v1, const glm::vec3 &v2);

static void mmOBJLoader(
	const char* file,
	std::vector<glm::vec3> *vertices,
	std::vector<glm::vec3> *normals,
	std::vector<glm::vec2> *uvs,
	std::vector<uint16_t>  *indices)
{
	char *buffer;
	mmReadFile(file, &buffer);

	std::stringstream ss;
	ss << buffer;

	if (NULL != buffer)
		free(buffer);

	std::vector<uint32_t> vert_indices, uv_indices, norm_indices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec3> temp_normals;
	std::vector<glm::vec2> temp_uvs;

	bool hasUV{false}, hasNormal{false};

	std::string line;
	while (std::getline(ss, line))
	{
		if (line[0] == '#') continue;

		if (line[0] == 'v')
		{
			switch(line[1])
			{
				case ' ':
						{
							glm::vec3 v;
							sscanf(line.c_str()+1, "%f %f %f", &v.x, &v.y, &v.z);
							temp_vertices.emplace_back(v);
						}
						break;

				case 'n':
						{
							hasNormal = true;
							glm::vec3 n;
							sscanf(line.c_str()+2, "%f %f %f", &n.x, &n.y, &n.z);
							temp_normals.emplace_back(n);
						}
						break;

				case 't':
						{
							hasUV = true;
							glm::vec2 t;
							sscanf(line.c_str()+2, "%f %f", &t.x, &t.y);
							temp_uvs.emplace_back(t);
						}
						break;
			}
		}
		else if (line[0] == 'f')
		{
			uint16_t v_i[3]{0}, vn_i[3]{0}, vt_i[3]{0};

			// find number of slashes
			uint8_t numOfSlashes{0};
			for (const auto& chr: line)
				if (chr == '/') ++numOfSlashes;
			switch(numOfSlashes)
			{
				case 0:
				{
					sscanf( line.c_str()+2, "%hd %hd %hd",
							&v_i[0],
							&v_i[1],
							&v_i[2]);
				} break;

				case 3:
				{
					sscanf(	line.c_str()+2, "%hd/%hd %hd/%hd %hd/%hd",
							&v_i[0], &vt_i[0],
							&v_i[1], &vt_i[1],
							&v_i[2], &vt_i[2] );
				} break;

				case 6:
				{
					if (hasNormal && hasUV)
					{
						sscanf( line.c_str()+2, "%hd/%hd/%hd %hd/%hd/%hd %hd/%hd/%hd",
							&v_i[0], &vt_i[0], &vn_i[0],
							&v_i[1], &vt_i[1], &vn_i[1],
							&v_i[2], &vt_i[2], &vn_i[2] );
					}
					else
					{
						sscanf( line.c_str()+2, "%hd//%hd %hd//%hd %hd//%hd",
							&v_i[0], &vn_i[0],
							&v_i[1], &vn_i[1],
							&v_i[2], &vn_i[2] );
					}
				} break;

				case 9:
				{
					sscanf( line.c_str()+2, "%hd//%hd//%hd %hd//%hd//%hd %hd//%hd//%hd",
							&v_i[0], &vt_i[0], &vn_i[0],
							&v_i[1], &vt_i[1], &vn_i[1],
							&v_i[2], &vt_i[2], &vn_i[2] );
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

  for (const auto& vert_i: vert_indices)
    vertices->emplace_back(temp_vertices[vert_i-1]);

  if (hasNormal)
  {
    for (const auto& norm_i: norm_indices)
      normals->emplace_back(temp_normals[norm_i-1]);
  }

  if (hasUV)
  {
    for (const auto& uv_i: uv_indices)
      uvs->emplace_back(temp_uvs[uv_i-1]);
  }

  printf("loaded gameobject: %s\n", file);
}

static bool isSimilarVec2(const glm::vec2 &v1, const glm::vec2 &v2)
{
	return 	fabs( v1.x-v2.x ) < 0.01f &&
					fabs( v1.y-v2.y ) < 0.01f;
}

static bool isSimilarVec3(const glm::vec3 &v1, const glm::vec3 &v2)
{
	return 	fabs( v1.x-v2.x ) < 0.01f &&
					fabs( v1.y-v2.y ) < 0.01f &&
					fabs( v1.z-v2.z ) < 0.01f;
}


#endif
