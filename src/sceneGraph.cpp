#include "sceneGraph.h"

SceneGraph::SceneGraph(){

}

void SceneGraph::addNode(Renderable* newRenderable, TreeNode* parentNode){
    std::unique_ptr<TreeNode> newNode = std::make_unique<TreeNode>();
    newNode->objectData = newRenderable;
    newNode->isModified = true;
    newNode->nodeType = NODETYPES::OBJECT_NODE;
    workingGraph.addNode(std::move(newNode));
}


void SceneGraph::addNode(Transformation* newTransformation, TreeNode* parentNode){
    return;
}

std::vector<TreeNode*>& SceneGraph::getSceneGraph(){
    return workingGraph.getLeaves();
}