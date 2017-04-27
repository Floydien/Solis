#include "camera.h"


void Camera::input(float delta, SolisDevice *device) 
{
    oldPos = pos;
    oldDir = dir;

    if(device->keyPressed(GLFW_KEY_S)) 
        pos -= dir * delta * speed;

    if(device->keyPressed(GLFW_KEY_W)) 
        pos += dir * delta * speed;

    if(device->keyPressed(GLFW_KEY_A)) 
        pos -= glm::normalize(glm::cross(dir, up)) * delta * speed;

    if(device->keyPressed(GLFW_KEY_D)) 
        pos += glm::normalize(glm::cross(dir, up)) * delta * speed;

    if(glfwGetMouseButton(device->getWindow(), GLFW_MOUSE_BUTTON_2)) 
    {
        glfwSetInputMode(device->getWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        double xdelta, ydelta;
        double halfWidth = device->getWidth() / 2.0;
        double halfHeight = device->getHeight() / 2.0;

        glfwGetCursorPos(device->getWindow(), &xdelta, &ydelta);
        xdelta -= halfWidth;
        ydelta -= halfHeight;

        // is not time(delta) dependent
        yaw += xdelta * 0.05f;
        pitch -= ydelta * 0.05f; // + is inverted camera.. ewww

        glfwSetCursorPos(device->getWindow(), halfWidth,  halfHeight);
    }
    else
    {
        glfwSetInputMode(device->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    if(device->keyPressed(GLFW_KEY_UP)) 
        pitch += rotationSpeed * delta;

    if(device->keyPressed(GLFW_KEY_DOWN)) 
        pitch -= rotationSpeed * delta;

    if(device->keyPressed(GLFW_KEY_LEFT)) 
        yaw -= rotationSpeed * delta;

    if(device->keyPressed(GLFW_KEY_RIGHT)) 
        yaw += rotationSpeed * delta;

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

glm::mat4 Camera::getView() const 
{
    return glm::lookAt(
            pos,
            dir + pos,
            up);
}

glm::mat4 Camera::getProjection() const 
{
    return projection;
}

bool Camera::hasChanged() const 
{
    if(changed) 
    {
        changed = false;
        return true;
    }
    return false;
}
