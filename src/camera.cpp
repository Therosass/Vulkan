#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <regex>

#include <iostream>

Camera::Camera(){
    resetCamera();
    this->cameraBuffer = -1;
}

const glm::vec3& Camera::getCameraPos(){
    return cameraPos;
}
const glm::vec3& Camera::getCameraUp(){
    return cameraUp;
}
const glm::vec3& Camera::getCameraLookPos(){
    return lookPos;
}

void Camera::updateSwapchainExtent(VkExtent2D newSwapchainExtent){
    swapchainExtent = newSwapchainExtent;
}

void Camera::setCameraBuffer(int bufferID){
    cameraBuffer = bufferID;
}

int Camera::getCameraBuffer(){
    return cameraBuffer;
}

void Camera::updateCamera(double x, double y){

	xPos += x/50;
    yPos += y/50;

    if(xPos > 360){
        xPos = 0;
    }
    if(xPos < 0){
        xPos = 360;
    }
    if(yPos > 85){
        yPos = 85;
    }
    if(yPos < -85){
        yPos = -85;
    }

    cameraDir = glm::vec3(0.0f,0.0f,1.0f);
    cameraRight = glm::vec3(1.0f,0.0f,0.0f);
    // Rotate y
    glm::quat turnQuat = glm::angleAxis(glm::radians(float(-xPos)), glm::normalize(glm::vec3(0.0f,1.0f,0.0f)));
    // Rotate x
    glm::quat turnQuat2 = glm::angleAxis(glm::radians(float(yPos)), glm::normalize(glm::vec3(1.0f,0.0f,0.0f)));

    cameraDir = turnQuat * turnQuat2 * cameraDir; 

    // Rotate x
    //cameraDir = turnQuat * cameraDir;
    //cameraUp =  glm::normalize(glm::cross(cameraRight, cameraDir));
    //if(yPos < 0){
    //    cameraUp *= -1;
    //}

    lookPos = cameraPos + cameraDir;
    cameraRight += cameraPos;
    //cameraUp += cameraPos;

    //std::cout << "Camera Data" << std::endl;
    //std::cout << "Rotations (X:Y) " << xPos << " " << yPos << std::endl;
    //std::cout << "Camera Direction " << cameraDir.x << " " << cameraDir.y << std::endl;
    //std::cout << "Camera Position " << cameraPos.x << " " << cameraPos.y << " " << cameraPos.z << std::endl;
    //std::cout << "Look position " << lookPos.x << " " << lookPos.y << " " << lookPos.z << std::endl;
    //std::cout << "Camera up " << cameraUp.x << " " <<  cameraUp.y << " " <<  cameraUp.z << std::endl;
    //std::cout << "Camera right " << cameraRight.x << " " <<  cameraRight.y << " " <<  cameraRight.z << std::endl;
}

void Camera::updateCamera(enum EVENTS movementEvent){
    switch(movementEvent){
        case EVENTS::CAMERA_FORWARD:
        {
            glm::vec3 cameraMovement = (lookPos - cameraPos);
            cameraPos += cameraMovement * 0.01f;
            lookPos += cameraMovement * 0.01f;   
            break;
        }
        case EVENTS::CAMERA_BACKWARD:
        {
            glm::vec3 cameraMovement = (lookPos - cameraPos);
            cameraPos -= cameraMovement * 0.01f;
            lookPos -= cameraMovement * 0.01f;
            break;
        }
        case EVENTS::CAMERA_STRAFE_RIGHT:
        {
            glm::vec3 cameraMovement = glm::normalize(glm::cross((lookPos - cameraPos),cameraUp));
            cameraPos += cameraMovement * 0.01f;
            lookPos += cameraMovement * 0.01f;
            break;
        }
        case EVENTS::CAMERA_STRAFE_LEFT:
        {
            glm::vec3 cameraMovement = glm::normalize(glm::cross((lookPos - cameraPos),cameraUp));
            cameraPos -= cameraMovement * 0.01f;
            lookPos -= cameraMovement * 0.01f;
            break;
        }
        default:
        break;
    }
}

UniformBufferObject Camera::getCurrentCamera(){
    UniformBufferObject currentCamera;
    currentCamera.model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    currentCamera.view = glm::lookAt(getCameraPos(), getCameraLookPos(), getCameraUp());
    currentCamera.proj = glm::perspective(glm::radians(45.0f), swapchainExtent.width / (float) swapchainExtent.height, 0.1f, 10.0f);
    currentCamera.proj[1][1] *= -1;

    return currentCamera;
}



void Camera::resetCamera(){
    xPos = 0;
    yPos = 0;
    cameraPos = glm::vec3(0.0f,0.0f,-3.0f);
    cameraUp = glm::vec3(0.0f,1.0f,0.0f);
    cameraRight = glm::normalize(glm::cross(cameraPos,cameraUp));
    lookPos = glm::vec3(0.0f,0.0f,-2.0f);

    //std::cout << "Camera Direction " << cameraDir.x << " " << cameraDir.y << std::endl;
    //std::cout << "Camera Position " << cameraPos.x << " " << cameraPos.y << " " << cameraPos.z << std::endl;
    //std::cout << "Look position " << lookPos.x << " " << lookPos.y << " " << lookPos.z << std::endl;
    //std::cout << "Camera up " << cameraUp.x << " " <<  cameraUp.y << " " <<  cameraUp.z << std::endl;
    //std::cout << "Camera right " << cameraRight.x << " " <<  cameraRight.y << " " <<  cameraRight.z << std::endl;
}

glm::vec3 Camera::readNewCameraPos(std::string posAsString, std::error_code* ec){
    std::regex re = std::regex("((?:1.0|0\\.[0-9]+),?)", std::regex::ECMAScript);
    std::smatch matches;
    std::regex_search(posAsString,matches,re);
    unsigned int coordsGiven = 0;
    glm::vec3 returnCoords = glm::vec3{0.0f,0.0f,0.0f};

    while (std::regex_search(posAsString, matches, re))
    {
        coordsGiven++;
        if(coordsGiven > 3){
            if(ec){
                *ec = RendererErrors::InvalidCameraPos;
            }
            return glm::vec3{0.0f,0.0f,0.0f};
        }


        returnCoords[coordsGiven - 1] = std::stod(matches[1]);

        posAsString = matches.suffix().str();
    }

    return returnCoords;
}   