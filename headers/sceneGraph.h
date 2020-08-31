#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <boost/thread/condition_variable.hpp>

#include "renderable.h"
#include "renderer.h"
#include "treeGraph.h"


class Transformation;

/* This class handles the creation and update of the scenegraph
*
*   Main functions:
*       - adding new nodes to be rendered
*       - updating the renderable tree nodes after game logic finished modifying them
*       - notifying the renderer that the render process can begin
*       - 
*/ 

/*
*   Basic implementation:
*       - Be able to add new nodes to rootNode
*       - Be able to update the tree
*       - Be able to signal renderer that the current scene is now ready
*
*/

class SceneGraph{
public:
    SceneGraph();
    ~SceneGraph(){};
    void init(Renderer* renderer);
    void addNode(Renderable* newRenderable, Renderable* parentRenderable = nullptr);
    void addNode(Transformation newTransformation);
    void updateNodes();
private:
    Renderer* renderer;
    TreeGraph workingGraph;
    TreeGraph renderGraph;
    boost::condition_variable* cv;
    void notifyRenderer();



};

#endif