    #ifndef KEYHANDLER_H
#define KEYHANDLER_H

#include <unordered_map>

/*
*   MASKS
*/

constexpr int16_t MODIFIER_CTRL   =0x0000000100000000;
constexpr int16_t MODIFIER_SHIFT  =0x0000001000000000;
constexpr int16_t MODIFIER_ALT    =0x0000010000000000;

struct keyData{
    bool isHeld;
    std::string actionBound;
};



class keyHandler{
public:
    keyHandler();
    keyData* getKeyState(char16_t keyRequested);
    void initKeys();
protected:

private:
    std::string configFilePath = "config/keys.ini";
    void readConfig(std::string filePath);
    bool isInitialized = false;
    std::unordered_map<char16_t,keyData> keyMap;
    void check_modifiers(char16_t &key, std::string text);

};

#endif
