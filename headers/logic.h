#ifndef LOGIC_H
#define LOGIC_H
#include "module.h"
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class SceneGraph;

class Logic : public Module{
public:
    Logic();
    ~Logic();
    //void init(std::shared_ptr<Core> engineCore);
    void update();
    void loadModel(std::string path);
    void setSceneGraph(std::shared_ptr<SceneGraph> newSceneGraph, std::shared_ptr<SceneGraph> newUISceneGraph);

private:

    int test = 0;

    std::shared_ptr<SceneGraph> sceneGraph;
    std::shared_ptr<SceneGraph> uiSceneGraph;

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