#ifndef _MM_MESH_H_
#define _MM_MESH_H_

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <cstdint>
#include <iostream>
#include <vector>
#include "MM_Utility.h"

struct Mesh
{
  enum {POSITION, NORMAL, UV, INDEX, NUM_BUFFERS};
  u32 VAO{0}, VBO[NUM_BUFFERS]{0};
  s16 drawCount{0};

  void draw()
  {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glBindVertexArray(VAO);
    //glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, drawCount);
    glEnable(GL_BLEND);
  }

  Mesh() = default;
  Mesh(const char* file)
  {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<u16>  indices;
    loadOBJ(file, &vertices, &normals, &uvs, &indices);

    drawCount = (u16)vertices.size();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate buffers
    glGenBuffers(NUM_BUFFERS, VBO);

    // POSITION
    glBindBuffer(GL_ARRAY_BUFFER, VBO[POSITION]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
     
    if (uvs.size() > 0)
    {
      // TEXTCOORD
      glBindBuffer(GL_ARRAY_BUFFER, VBO[UV]);
      glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(uvs[0]), &uvs[0], GL_STATIC_DRAW);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }

    if (normals.size() > 0)
    {
    // NORMAL
    glBindBuffer(GL_ARRAY_BUFFER, VBO[NORMAL]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals[0]), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
    // INDEX
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[INDEX]);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);
  }
  ~Mesh()
  {
    //glDeleteBuffers(NUM_BUFFERS, VBO);
    //glDeleteVertexArrays(1, &VAO);
  }
};

#endif
