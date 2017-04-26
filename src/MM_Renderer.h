#pragma once

#include "MM_Typedefs.h"
#include "MM_GameObject.h"
#include <vector>

// @Note: only place where graphics code is run. ONLY place.

struct Renderer
{
  struct MeshObject { u32 VAO; u32 count; f32 type; };

  std::vector<GameObject>  gameobjects;

  enum {POSITION, NORMAL, UV, INDEX, NUM_BUFFERS};

  void drawAll()
  {
    for (const auto& gameobject: gameobjects)
    {
      // glUseProgram(gameobject.shader.program);
      // gameobject.texture.bind(0);

      // glUniformMatrix4fv(uniform[MVP], 1, GL_FALSE, &mvp[0][0]);

      // glBindVertexArray(mesh.VAO);
      // glDrawArrays(mesh.type, 0, mesh.count);
      //glDrawElements(mesh.type, mesh.count, GL_UNSIGNED_INT, 0);
    }
  }

  void addGameObject(GameObject &gameobject)
  {

    // Shader setup







    // // Mesh setup
    // u32 VAO{0}, VBO[NUM_BUFFERS]{0};

    // gameobject.drawCount = gameobject.positions.size();

    // glGenVertexArrays(1, &VAO);
    // glBindVertexArray(VAO);

    // glGenBuffers(1, &VBO[POSITION]);
    // glBindBuffer(GL_ARRAY_BUFFER, VBO[POSITION]);
    // glBufferData(GL_ARRAY_BUFFER, gameobject.positions.size() * sizeof(gameobject.positions[0]), &gameobject.positions[0], GL_STATIC_DRAW);
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // if (!gameobject.uvs.empty())
    // {
    //   glGenBuffers(1, &VBO[UV]);
    //   glBindBuffer(GL_ARRAY_BUFFER, VBO[UV]);
    //   glBufferData(GL_ARRAY_BUFFER, gameobject.positions.size() * sizeof(gameobject.uvs[0]), &gameobject.uvs[0], GL_STATIC_DRAW);
    //   glEnableVertexAttribArray(1);
    //   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    // }

    // if (!gameobject.normals.empty())
    // {
    //   glGenBuffers(1, &VBO[NORMAL]);
    //   glBindBuffer(GL_ARRAY_BUFFER, VBO[NORMAL]);
    //   glBufferData(GL_ARRAY_BUFFER, gameobject.normals.size() * sizeof(gameobject.normals[0]), &gameobject.normals[0], GL_STATIC_DRAW);
    //   glEnableVertexAttribArray(2);
    //   glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // }

    // if (!gameobject.indices.empty())
    // {
    //   glGenBuffers(1, &VBO[INDEX]);
    //   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[INDEX]);
    //   glBufferData(GL_ELEMENT_ARRAY_BUFFER, gameobject.indices.size() * sizeof(gameobject.indices[0]), &gameobject.indices[0], GL_STATIC_DRAW);
    // }


    // // Add to buffer
    // MeshObject m;
    // m.VAO = VAO;
    // m.count = gameobject.positions.size();
    // m.type = gameobject.drawType;
    // gameobjects.emplace_back(m);
  }
};
