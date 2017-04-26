#ifndef _MM_CAMERA_H_
#define _MM_CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "MM_Typedefs.h"

struct Camera
{
  vec3 position{0, 0, -5};
  vec3 forward{0, 0, 1};
  vec3 up{0, 1, 0};

  f32 fov{120.0f};
  f32 aspectRatio{1.0f};
  f32 zNear{0.01f};
  f32 zFar{1000.0f};
  mat4 perspective{glm::perspective(fov, aspectRatio, zNear, zFar)};

  f32 moveSpeed{0.02f};
  Camera() = default;
  mat4 getViewProjection() const
  {
    return perspective * glm::lookAt(position, position + forward, up);
  }
  void updatePerspective()
  {
    perspective = glm::perspective(fov, aspectRatio, zNear, zFar);
  }
};

#endif
