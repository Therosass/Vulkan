#ifndef KEYHANDLER_H
#define KEYHANDLER_H

#include <unordered_map>
#include "module_enums.h"
#include "error.h"
#include <vector>

// TODO: CLEAR UP ACTION / EVENT NAMING

/*
*   MASKS
*/

enum EVENTS;

constexpr int32_t MODIFIER_CTRL   =0x00100000;
constexpr int32_t MODIFIER_SHIFT  =0x01000000;
constexpr int32_t MODIFIER_ALT    =0x10000000;

struct keyData{
    int32_t keyNumber;
    bool isHeld;
    enum EVENTS actionBound;
};



class KeyHandler{
public:
    KeyHandler();
    void initKeys();
    std::vector<keyData> getHeldKeys();
    static void setKeyState(int32_t button, bool state);
    void bindEventToKey(int32_t button, enum EVENTS eventToBind, std::error_code* ec = nullptr);
    void unbindKey(int32_t button);
    void printKeyData(int32_t key);
    void printKeyData(keyData key);

protected:

private:
    std::string configFilePath = "config/keys.ini";
    void readConfig(std::string filePath = "");
    bool isInitialized = false;
    inline static std::unordered_map<int32_t,keyData> keyMap;
    void check_modifiers(int32_t &key, std::string text);

};

#endif
