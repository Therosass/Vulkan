#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <module_enums.h>
#include "error.h"
#include "renderable.h"

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
    UniformBufferObject getCurrentCamera();
    void setCameraBuffer(int bufferID);
    int getCameraBuffer();
    void updateSwapchainExtent(VkExtent2D newSwapchainExtent);
    void updateCamera(double x, double y);
    void updateCamera(enum EVENTS movementEvent);
    void resetCamera();

private:

    double xPos;
    double yPos;
    int camerBuffer = -1;
    glm::vec3 cameraPos;
    glm::vec3 cameraDir;
    glm::vec3 cameraRight;
    glm::vec3 cameraUp;
    glm::vec3 lookPos;

    VkExtent2D swapchainExtent;
    int cameraBuffer;

    static glm::vec3 readNewCameraPos(std::string posAsString, std::error_code* ec = nullptr);
};

#endif