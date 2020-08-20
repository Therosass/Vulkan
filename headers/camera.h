#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <module_enums.h>
#include "error.h"

/* Camera implementation, an instance of this is created inside renderer class
*
*   EVENTS SUPPORTED
*       
*       CAMERA_FORWARD 
*       CAMERA_BACKWARD
*       CAMERA_STRAFE_LEFT
*       CAMERA_STRAFE_RIGHT
*       CAMERA_RESET
*       MOUSE_EVENT
*/




class Camera{
public:
    Camera();
    const glm::vec3& getCameraPos();
    const glm::vec3& getCameraUp();
    const glm::vec3& getCameraLookPos();
    void updateCamera(double x, double y);
    void updateCamera(enum EVENTS movementEvent);
    void resetCamera();

private:

    double xPos;
    double yPos;
    glm::vec3 cameraPos;
    glm::vec3 cameraDir;
    glm::vec3 cameraRight;
    glm::vec3 cameraUp;
    glm::vec3 lookPos;

    static glm::vec3 readNewCameraPos(std::string posAsString, std::error_code* ec = nullptr);
};

#endif