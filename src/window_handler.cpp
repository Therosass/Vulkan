#include "window_handler.h"

#include "core.h"

double WindowHandler::XP = 0;
double WindowHandler::YP = 0;
    
WindowHandler::WindowHandler(){
    this->moduleRole = MODULES::WINDOW;
}

void WindowHandler::error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void WindowHandler::mouse_callback(GLFWwindow* window, double xPos, double yPos){
    XP += xPos;
    YP += yPos;
    glfwSetCursorPos(window, 0, 0);
}

void WindowHandler::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    bool state = false;

    if(key > 64 && key < 91){
        key += 32;
    }

    switch(action){
        case GLFW_PRESS:
            state = true;
        break;

        case GLFW_RELEASE:
            state = false;
        break;

        default:
        return;
    }

    keyHandler.setKeyState(key, state);

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


void WindowHandler::init(Core* engineCore){
	this->engineCore = engineCore;
	initGLFW();
}

GLFWwindow* WindowHandler::getWindow(){
	return window;
}


void WindowHandler::getWindowEvents(){
	glfwPollEvents();
	if(windowResizedFlag){
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        std::cout << "window size is: " << windowWidth << ":" << windowHeight << std::endl;
        sendDataPacket(EVENTS::WINDOW_RESIZE, MODULES::RENDERER, std::pair<int,int>(100,150));
	    windowResizedFlag = false;
	}
    auto keys = keyHandler.getHeldKeys();
    for(auto key : keys){
        switch(key.actionBound){
            case EVENTS::CAMERA_FORWARD ... EVENTS::CAMERA_RESET:
                sendMessage(key.actionBound,MODULES::RENDERER);
                break;
            case EVENTS::LOAD_MODEL:
                sendMessage(key.actionBound,MODULES::LOGIC);
                break;
            default:
                break;
            
        }
    }
    if(XP != 0 || YP != 0){
        sendDataPacket(EVENTS::CURSOR_MOVE,MODULES::RENDERER,std::pair<double,double>(XP,YP));
        XP = 0;
        YP = 0;
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
	window = glfwCreateWindow(windowWidth, windowHeight, "Vulcanicus", NULL, NULL);
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
    app->windowWidth = width;
    app->windowHeight = height;
}


void WindowHandler::receiveMessage(){

}