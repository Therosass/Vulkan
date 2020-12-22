#include "sceneGraph.h"

//TODO: total rework, with proper memory management, and a memory allocation lib

SceneGraph::SceneGraph(){

}

void SceneGraph::addNode(std::shared_ptr<Renderable> newRenderable, TreeNode* parentNode){
    std::unique_ptr<TreeNode> newNode = std::make_unique<TreeNode>();
    newNode->objectData = newRenderable;
    newNode->isModified = true;
    newNode->nodeType = NODETYPES::OBJECT_NODE;
    workingGraph.addNode(std::move(newNode));
}


void SceneGraph::addNode(std::shared_ptr<Transformation> newTransformation, TreeNode* parentNode){
    return;
}

const std::vector<TreeNode*>& SceneGraph::getSceneGraph(){
    return workingGraph.getLeaves();
}