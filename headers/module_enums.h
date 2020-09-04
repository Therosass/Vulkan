#ifndef MODULE_ENUMS_H
#define MODULE_ENUMS_H

#include <boost/cstdint.hpp>
#include <unordered_map>
namespace ENUMS{
    enum DATATYPES{
        UNKNOWN_TYPE,
        CHAR,
        FLOAT,
        DOUBLE,
        INT,
        BYTES,
        STDSTRING
    };
}

namespace CONTAINERS{

    constexpr int32_t is_array = 0x10000000;
    constexpr int32_t is_list = 0x20000000;

}

enum ERRORCODES{
    INVALID_ERROR
};

enum EVENTS{
#ifdef DEBUG_MODE_RENDERER
    WINDOW_RESIZE_SUCCESSFUL,
#endif
    WINDOW_RESIZE,
    CURSOR_MOVE,
    KEYBOARD_EVENT,
	CAMERA_FORWARD,
	CAMERA_BACKWARD,
	CAMERA_STRAFE_RIGHT,
    CAMERA_STRAFE_LEFT,
    CAMERA_RESET
};

namespace ENUMMAP {
    static std::unordered_map<std::string, enum EVENTS> eventMap = { {"WINDOW_RESIZE", EVENTS::WINDOW_RESIZE}, \
                                                         {"CURSOR_MOVE", EVENTS::CURSOR_MOVE}, \
                                                         {"KEYBOARD_EVENT", EVENTS::KEYBOARD_EVENT}, \
                                                         {"CAMERA_FORWARD", EVENTS::CAMERA_FORWARD}, \
                                                         {"CAMERA_BACKWARD", EVENTS::CAMERA_BACKWARD}, \
                                                         {"CAMERA_RESET", EVENTS::CAMERA_RESET}, \
                                                         {"CAMERA_STRAFE_LEFT", EVENTS::CAMERA_STRAFE_LEFT}, \
                                                         {"CAMERA_STRAFE_RIGHT", EVENTS::CAMERA_STRAFE_RIGHT} \
                                                        };

    static std::unordered_map<enum EVENTS, std::string> reverseEventMap = { {EVENTS::WINDOW_RESIZE, "WINDOW_RESIZE"}, \
                                                         {EVENTS::CURSOR_MOVE, "CURSOR_MOVE"}, \
                                                         {EVENTS::KEYBOARD_EVENT, "KEYBOARD_EVENT"}, \
                                                         {EVENTS::CAMERA_FORWARD, "CAMERA_FORWARD"}, \
                                                         {EVENTS::CAMERA_BACKWARD ,"CAMERA_BACKWARD"}, \
                                                         {EVENTS::CAMERA_RESET ,"CAMERA_RESET"}, \
                                                         {EVENTS::CAMERA_STRAFE_LEFT, "CAMERA_STRAFE_LEFT"}, \
                                                         {EVENTS::CAMERA_STRAFE_RIGHT, "CAMERA_STRAFE_RIGHT"} \
                                                        };

}

enum MODULES{
    CORE = 0x00000000,
    WINDOW = 0x00000001,
    RENDERER = 0x00000002,
    MESSAGEHANDLER = 0x000000003
};



#endif