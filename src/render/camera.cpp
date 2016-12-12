#include "camera.h"


void Camera::input(float delta, SolisDevice *device) 
{
    oldPos = pos;
    oldDir = dir;

    if(device->keyPressed(GLFW_KEY_S)) {
        pos -= dir * delta * speed;
    }
    if(device->keyPressed(GLFW_KEY_W)) {
        pos += dir * delta * speed;
    }
    if(device->keyPressed(GLFW_KEY_A)) {
        pos -= glm::normalize(glm::cross(dir, up)) * delta * speed;
    }
    if(device->keyPressed(GLFW_KEY_D)) {
        pos += glm::normalize(glm::cross(dir, up)) * delta * speed;
    }

    if(device->keyPressed(GLFW_KEY_UP)) {
        pitch += rotationSpeed * delta;
    }
    if(device->keyPressed(GLFW_KEY_DOWN)) {
        pitch -= rotationSpeed * delta;
    }
    if(device->keyPressed(GLFW_KEY_LEFT)) {
        yaw -= rotationSpeed * delta;
    }
    if(device->keyPressed(GLFW_KEY_RIGHT)) {
        yaw += rotationSpeed * delta;
    }

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    dir.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    dir.y = glm::sin(glm::radians(pitch));
    dir.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    //dir = glm::normalize(dir);

    if(oldPos != pos || oldDir != dir)
        changed = true;
}

glm::mat4 Camera::getViewProjection() const
{
    glm::mat4 vp(projection);
    glm::mat4 view = glm::lookAt(
            pos,
            dir + pos,
            up);

    return vp * view;
}

bool Camera::hasChanged() const {
    if(changed) {
        changed = false;
        return true;
    }
    return false;
}
