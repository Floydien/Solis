#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "solisdevice.h"

class Camera {
public:
    Camera(float fov, float aspectRatio, float nearZ, float farZ, const glm::vec3 pos = glm::vec3(), float speed = 1.0f) :
        Camera(glm::perspective(glm::radians(fov),aspectRatio, nearZ, farZ), pos, speed) {};

    Camera(const glm::mat4& proj, const glm::vec3 pos = glm::vec3(), float speed = 1.0f) :
        projection(proj),
        speed(speed),
        rotationSpeed(4.0f),
        pos(pos),
        dir(glm::vec3(0,0,1)),
        up(glm::vec3(0,1,0)),
        pitch(0),
        yaw(90),
        changed(true){}

    void input(float, SolisDevice *);

    glm::mat4 getViewProjection() const;
    glm::mat4 getView() const;
    glm::mat4 getProjection() const;
    inline const glm::vec3 &getPosition() const { return pos; };

    inline void setSpeed(float value) { speed = value; };
    inline void setRotationSpeed(float value) { rotationSpeed = value; };

    //checks if the camera has changed since the last call
    bool hasChanged() const;
private:
    const glm::mat4 projection;

    float speed;
    float rotationSpeed;
    glm::vec3 pos;
    glm::vec3 dir;
    glm::vec3 up;

    float pitch;
    float yaw;

    glm::vec3 oldPos;
    glm::vec3 oldDir;

    mutable bool changed;
};
