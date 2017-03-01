#include "Camera.hpp"

#include <algorithm>
#include <iostream>

#include <glm/ext.hpp>

namespace world {

void Camera::init(PerspectiveState& initialPerspective, data::CameraState& cameraState) {
  this->perspectiveState = initialPerspective;
  this->cameraState = cameraState;
  this->updateViewMatrix();
  this->updateProjectionMatrix();
  this->updateViewProjectionMatrix();
}

void Camera::move(glm::vec3 movement) {
  this->cameraState.lookAt +=
      glm::vec3(glm::rotate(glm::mat4(1), -cameraState.rotationAroundY, glm::vec3(0, 1, 0)) * glm::vec4(movement, 1.f));
  this->viewChanged = true;
}

void Camera::zoom(float distanceDelta) {
  this->cameraState.distance = std::max(this->cameraState.distance - distanceDelta, 0.f);
  this->viewChanged = true;
}

void Camera::rotateAroundY(float angleDelta) {
  this->cameraState.rotationAroundY -= angleDelta;
  this->viewChanged = true;
}

void Camera::rotateAroundX(float angleDelta) {
  if (this->cameraState.rotationAroundX + angleDelta >= (float)M_PI / 2.f ||
      this->cameraState.rotationAroundX + angleDelta <= (float)-M_PI / 2.f) {
    return;
  }

  this->cameraState.rotationAroundX += angleDelta;
  this->viewChanged = true;
}

void Camera::updateAspect(float aspect) {
  this->perspectiveState.aspect = aspect;
  this->updateProjectionMatrix();
  this->updateViewProjectionMatrix();
}

float Camera::getFovy() {
  return this->perspectiveState.fovy;
}

glm::vec3 Camera::getPosition() {
  if (this->viewChanged) {
    this->updateViewProjectionMatrix();
  }
  return this->cameraState.lookAt + glm::vec3(this->cameraPosition);
}

glm::vec3 Camera::getLookAt() {
  return this->cameraState.lookAt;
}

float Camera::getRotationAroundY() {
  return this->cameraState.rotationAroundY;
}

float Camera::getDistance() {
  return this->cameraState.distance;
}

glm::mat4 Camera::getProjectionMatrix() {
  return this->projectionMatrix;
}

glm::mat4 Camera::getViewMatrix() {
  if (this->viewChanged) {
    this->updateViewProjectionMatrix();
  }
  return this->viewMatrix;
}

glm::mat4 Camera::getViewProjectionMatrix() {
  if (this->viewChanged) {
    this->updateViewProjectionMatrix();
  }
  return this->viewProjectionMatrix;
}

void Camera::updateViewMatrix() {

  cameraPosition = glm::vec4(0, 0, -cameraState.distance, 1);
  cameraPosition = glm::rotate(glm::mat4(1), cameraState.rotationAroundX, glm::vec3(1, 0, 0)) * cameraPosition;
  cameraPosition = glm::rotate(glm::mat4(1), -cameraState.rotationAroundY, glm::vec3(0, 1, 0)) * cameraPosition;

  this->viewMatrix = glm::lookAt(this->cameraState.lookAt + glm::vec3(cameraPosition), this->cameraState.lookAt,
                                 glm::vec3(0.0, 1.0, 0.0));

  this->viewMatrixInverse = glm::inverse(this->viewMatrix);

  this->viewChanged = false;
}

void Camera::updateProjectionMatrix() {
  this->projectionMatrix = glm::perspective(this->perspectiveState.fovy, this->perspectiveState.aspect,
                                            this->perspectiveState.zNear, this->perspectiveState.zFar);

  this->projectionMatrixInverse = glm::inverse(this->projectionMatrix);
}

void Camera::updateViewProjectionMatrix() {
  if (this->viewChanged) {
    this->updateViewMatrix();
  }
  this->viewProjectionMatrix = this->getProjectionMatrix() * this->getViewMatrix();
}

glm::vec3 Camera::getRay(glm::vec2 point) {
  auto ray = glm::vec4(point.x, point.y, -1.f, 1.0f);

  ray = this->projectionMatrixInverse * ray;
  ray = glm::vec4(ray.x, -ray.y, -1.f, 0.0);
  ray = this->viewMatrixInverse * ray;
  ray.w = 0.f;
  ray = glm::normalize(ray);

  return glm::vec3(ray);
}
}