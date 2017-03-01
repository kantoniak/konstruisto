#ifndef WORLD_CAMERA_HPP
#define WORLD_CAMERA_HPP

#include <cmath>

#include <glm/gtc/matrix_transform.hpp>

#include "../data/CameraState.hpp"

namespace world {

struct PerspectiveState {
  float fovy, aspect, zNear, zFar;
};

class Camera {

public:
  void init(PerspectiveState& initialPerspective, data::CameraState& initialCamera);

  void move(glm::vec3 movement);
  void zoom(float distanceDelta);
  void rotateAroundY(float angleDelta);
  void rotateAroundX(float angleDelta);

  void updateAspect(float aspect);

  float getFovy();

  glm::vec3 getPosition();
  glm::vec3 getLookAt();
  float getRotationAroundY();
  float getDistance();

  glm::mat4 getProjectionMatrix();
  glm::mat4 getViewMatrix();
  glm::mat4 getViewProjectionMatrix();

  glm::vec3 getRay(glm::vec2 point);

protected:
  glm::vec4 cameraPosition;
  glm::mat4 projectionMatrix, viewMatrix, viewProjectionMatrix;

  // Cached
  glm::mat4 projectionMatrixInverse, viewMatrixInverse;

  PerspectiveState perspectiveState;
  data::CameraState cameraState;

  bool viewChanged = false;

  void updateProjectionMatrix();
  void updateViewMatrix();
  void updateViewProjectionMatrix();
};
}

#endif
