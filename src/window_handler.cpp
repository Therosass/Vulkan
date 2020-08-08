#include "window_handler.h"

#include "core.h"

WindowHandler::WindowHandler(){
    this->moduleRole = MODULES::WINDOW;
}

void WindowHandler::error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void WindowHandler::mouse_callback(GLFWwindow* window, double xPos, double yPos){
    
    /*
	*
	*	TO BE MOVED AND REWORKED
	*
	XP += xPos/1000;
    YP += yPos/1000;

    while(XP > 360){
        XP -= 360;
    }
    while(XP < 0){
        XP += 360;
    }
    if(YP > 85){
        YP = 85;
    }
    if(YP < -85){
        YP = -85;
    }
    cameraDir = glm::vec3(0.0f,0.0f,1.0f);
    glm::quat turnQuat = glm::angleAxis(float(-XP/glm::half_pi<float>()), glm::vec3(0.0f,1.0f,0.0f));
    cameraDir = turnQuat * cameraDir;
    turnQuat = glm::angleAxis(glm::half_pi<float>(), glm::vec3(0.0f,1.0f,0.0f));
    cameraRight = turnQuat * cameraDir;
    turnQuat = glm::angleAxis(float(YP/glm::half_pi<float>()), cameraRight);
    cameraUp =  glm::normalize(glm::cross(cameraRight, lookPos));
    cameraDir = turnQuat * cameraDir;
    lookPos = cameraPos + cameraDir;

    //cameraRight = cameraPos + ((glm::angleAxis(glm::half_pi<float>(),glm::vec3(0.0f,1.0f,0.0f))) * cameraDir);
    //cameraUp = glm::normalize(glm::cross(cameraRight, lookPos));
    cameraDir = lookPos - cameraPos;
    turnQuat = glm::angleAxis(float(YP/glm::half_pi<float>()), glm::vec3(0.0f,0.0f,1.0f));
    cameraDir = turnQuat * cameraDir;
    lookPos = cameraPos + cameraDir;
    lookPos = cameraPos + (turnQuat * (lookPos - cameraPos));
    cameraUp = glm::normalize(turnQuat * cameraUp);


    glfwSetCursorPos(window, 0, 0);
    
    while(cameraDir.x > 360){
        cameraDir.x -= 360;
    }
    while(cameraDir.x < 0){
        cameraDir.x += 360;
    }
    while(cameraDir.y > 360){
        cameraDir.y -= 360;
    }
    while(cameraDir.y < 0){
        cameraDir.y += 360;
    }

    std::cout << "Camera Data" << std::endl;
    std::cout << "Rotations (X:Y) " << XP << " " << YP << std::endl;
    std::cout << "Camera Direction " << cameraDir.x << " " << cameraDir.y << std::endl;
    std::cout << "Camera Position " << cameraPos.x << " " << cameraPos.y << " " << cameraPos.z << std::endl;
    std::cout << "Look position " << lookPos.x << " " << lookPos.y << " " << lookPos.z << std::endl;
    std::cout << "Camera up " << cameraUp.x << " " <<  cameraUp.y << " " <<  cameraUp.z << std::endl;
    std::cout << "Camera right " << cameraRight.x << " " <<  cameraRight.y << " " <<  cameraRight.z << std::endl;
	*
	*
	*/
}

void WindowHandler::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//
	//	REWORK
	//
    //if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
    //    glfwSetWindowShouldClose(window, GLFW_TRUE);
    //}
    //if(key == GLFW_KEY_W){
//
    //    auto cameraMov = glm::normalize(glm::vec3(cameraPos-lookPos));
    //    cameraMov /= 10;
    //    cameraPos -= cameraMov;
    //    lookPos -= cameraMov;
    //}
    //if(key == GLFW_KEY_A){
    //    auto cameraMov = cameraRight;
    //    cameraMov /= 10;
    //    cameraPos += cameraMov;
    //    lookPos += cameraMov;
    //}
    //if(key == GLFW_KEY_S){
//
    //    auto cameraMov = glm::normalize(glm::vec3(cameraPos-lookPos));
    //    cameraMov /= 10;
    //    cameraPos += cameraMov;
    //    lookPos += cameraMov;
    //}
    //if(key == GLFW_KEY_D){
    //    auto cameraMov = -cameraRight;
    //    cameraMov /= 10;
    //    cameraPos += cameraMov;
    //    lookPos += cameraMov;
    //}
    //if(key == GLFW_KEY_SPACE){
    //    cameraPos = glm::vec3(1.0f,0.0f,0.0f);
    //    cameraUp = glm::vec3(0.0f,1.0f,0.0f);
    //    XP = 0;
    //    YP = 0;
    //    cameraRight = glm::normalize(glm::cross(cameraPos,cameraUp));
    //    lookPos = glm::vec3(0.0f,0.0f,0.0f);
    //}
    //if(key == GLFW_KEY_C){
    //    std::string read;
    //    std::cin.clear();
    //    std::cin >> read;
    //    cameraPos = readNewCameraPos(read);
//
    //    
    //}
//
    //if (key == GLFW_MOUSE_BUTTON_LEFT) {
    //    if(GLFW_PRESS == action){
    //        printf("ASDSADDSADAS");
    //        lbutton_down = true;
    //    }
    //    else if(GLFW_RELEASE == action){
    //        lbutton_down = false;
    //    }
    //}
//
    //if(lbutton_down){
    //    double x;
    //    double y;
    //    glfwGetCursorPos(window,&x,&y);
    //    printf("%d %d", x, y);
    //}
}


void WindowHandler::start(Core* engineCore){
	this->engineCore = engineCore;
	initGLFW();
}

GLFWwindow* WindowHandler::getWindow(){
	return window;
}


void WindowHandler::getWindowEvents(){
	glfwPollEvents();
	if(windowResizedFlag){
		sendMessage(EVENTS::WINDOW_RESIZE, MODULES::RENDERER);
	    windowResizedFlag = false;
	}

}

std::pair<int,int> WindowHandler::getCurrentMousePos(){
    return currentMousePos;
}

void WindowHandler::setCurrentMousePos(std::pair<int,int> newMousePos){
    currentMousePos = newMousePos;
}


void WindowHandler::initGLFW(){
	if(!glfwInit()){
		printf("failed to init GLFW");
	}
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwSetErrorCallback(error_callback);
	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulcanicus", NULL, NULL);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported()){
        //glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        glfwSetCursorPosCallback(window, mouse_callback);
    }

	if (!window)
	{
		// Window or OpenGL context creation failed
	}
	glfwSetKeyCallback(window, key_callback);
}

void WindowHandler::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto app = reinterpret_cast<WindowHandler*>(glfwGetWindowUserPointer(window));
	app->windowResizedFlag = true;
}


void WindowHandler::receiveMessage(){

}