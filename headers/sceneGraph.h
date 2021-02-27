#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <boost/thread/condition_variable.hpp>

#include "renderable.h"
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
    void init();
    TreeNode* addNode(std::shared_ptr<Renderable> newRenderable, TreeNode* parentNode = nullptr);
    TreeNode* addNode(std::shared_ptr<TransformationMatrix> newTransformation, TreeNode* parentNode = nullptr);
    const std::vector<TreeNode*>& getSceneGraph();
    //void addNode(Group* newGroup);
    void updateNodes();
private:
    TreeGraph workingGraph;
    TreeGraph renderGraph;
    boost::condition_variable* cv;
    void notifyRenderer();



};

#endif