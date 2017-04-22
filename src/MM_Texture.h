#ifndef _MM_TEXTURE_H_
#define _MM_TEXTURE_H_

#include <stdio.h>

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define GLEW_STATIC
#include <GL/glew.h>

struct MMTexture
{
  GLuint texId;
  MMTexture(const char* file, GLfloat filtering)
  {
    GLint width, height, num_comp;
    GLubyte *image_data = stbi_load(file, &width, &height, &num_comp, 4);
    if (NULL == image_data)
      printf("Texture loading failed: %s\n",file);
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);
    printf("Texture loaded: %s\n",file);
  }

  ~MMTexture()
  {
    glDeleteTextures(1, &texId);
  }

  void bind(GLuint unit)
  {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texId);
  }

};

#endif
