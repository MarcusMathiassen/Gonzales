#ifndef _MM_CAMERA_H_
#define _MM_CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct Camera
{
  glm::vec3 position{0, 0, -5};
  glm::vec3 forward{0, 0, 1};
  glm::vec3 up{0, 1, 0};

  float fov{120.0f};
  float aspectRatio{1.0f};
  float zNear{0.01f};
  float zFar{1000.0f};
  glm::mat4 perspective{glm::perspective(fov, aspectRatio, zNear, zFar)};

  float moveSpeed{0.02f};
  Camera() = default;
  glm::mat4 getViewProjection() const
  {
    return perspective * glm::lookAt(position, position + forward, up);
  }
  void updatePerspective()
  {
    perspective = glm::perspective(fov, aspectRatio, zNear, zFar);
  }
};

#endif
