#ifndef LOGIC_H
#define LOGIC_H
#include "module.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Logic : public Module{
public:
    Logic();
    ~Logic();
    //void init(std::shared_ptr<Core> engineCore);
    void update();
    void loadModel(std::string path);

private:

    int test = 0;

    /*****
     * 
     * Module function implementation
     * 
     ****/


    //Load a model to RAM from file

    //Upload model from RAM to GPU memory
    void uploadModelToGPU(int modelID);

    void receiveMessage() override;

};

#endif //(LOGIC_H)