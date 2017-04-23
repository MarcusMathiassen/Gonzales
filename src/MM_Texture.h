#ifndef _MM_TEXTURE_H_
#define _MM_TEXTURE_H_

#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_PSD
#define STBI_NO_TGA
#define STBI_NO_GIF
#define STBI_NO_HDR
#define STBI_NO_PIC
#define STBI_NO_PNM
#include "stb_image.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <iostream>

struct MMTexture
{
  GLuint texId{0};
  MMTexture(const char* file, GLfloat filtering)
  {
    GLint width, height, num_comp;
    GLubyte *image_data = stbi_load(file, &width, &height, &num_comp, 4);
    if (NULL == image_data)
      printf("Texture loading failed: %s\n",file);
    glGenTextures(1, &texId);
    glActiveTexture(GL_TEXTURE0);
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

  MMTexture() = default;

  void bind(GLuint unit)
  {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texId);
  }

};

#endif
