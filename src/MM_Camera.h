#ifndef _MM_CAMERA_H_
#define _MM_CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct MMCamera
{
  glm::vec3 position{0, 0, -5};
  glm::vec3 forward{0, 0, 1};
  glm::vec3 up{0, 1, 0};

  float fov{70.0f};
  float aspect{1.0f};
  float zNear{0.01f};
  float zFar{1000.0f};
  glm::mat4 perspective{glm::perspective(fov, aspect, zNear, zFar)};

  float moveSpeed{0.02f};
  MMCamera() = default;
  glm::mat4 getViewProjection() const
  {
    return perspective * glm::lookAt(position, position + forward, up);
  }
};

MMCamera MMDefaultCamera;

static MMCamera& mmGetCamera()
{
  return MMDefaultCamera;
}

#endif
