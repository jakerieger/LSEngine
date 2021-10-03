#pragma once

#include "glad.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <vector>

enum CAMERA_MOVEMENT {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 60.0f;

class LSCamera
{
public:
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;

  float Yaw;
  float Pitch;
  float MovementSpeed;
  float MouseSensitivity;
  float Zoom;

  void Use(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

  void Use(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

  glm::mat4 GetViewMatrix();

  void ProcessKeyboard(CAMERA_MOVEMENT direction, float deltaTime);

  void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch);

  void ProcessMouseScroll(float yoffset);
private:
  void UpdateCameraVectors();
};

