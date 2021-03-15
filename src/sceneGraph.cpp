#include "sceneGraph.h"

//TODO: total rework, with proper memory management, and a memory allocation lib

SceneGraph::SceneGraph(){

}

TreeNode* SceneGraph::addNode(std::shared_ptr<Renderable> newRenderable, TreeNode* parentNode){
    if(parentNode == nullptr){
        std::shared_ptr<TransformationMatrix> newTransform = std::make_shared<TransformationMatrix>();
        newTransform->scale = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        newTransform->rotate = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        newTransform->translate = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };  
        parentNode = addNode(newTransform,workingGraph.getRoot());
    }
    std::unique_ptr<TreeNode> newNode = std::make_unique<TreeNode>();
    newNode->objectData = newRenderable;
    newNode->isModified = true;
    newNode->nodeType = NODETYPES::OBJECT_NODE;
    return workingGraph.addNode(std::move(newNode),parentNode);
}


TreeNode* SceneGraph::addNode(std::shared_ptr<TransformationMatrix> newTransformation, TreeNode* parentNode){
    std::unique_ptr<TreeNode> newNode = std::make_unique<TreeNode>();
    newNode->TRMatrix = newTransformation;
    newNode->isModified = true;
    newNode->nodeType = NODETYPES::TRANSFORMATION_NODE;
    TreeNode* nodeptr = workingGraph.addNode(std::move(newNode),parentNode);
    return nodeptr;
}

const std::vector<TreeNode*>& SceneGraph::getSceneGraph(){
    int count = 0;
    for(auto item : workingGraph.getLeaves()){
        if(item->isModified)
        {
            auto transform = workingGraph.getObjectTRMatrices()[count];
            transform->scale = {
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };
            transform->rotate = {
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };
            transform->translate = {
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };
            item->updateObject(transform);
            item->objectData->setTRMatrix(item->TRMatrix);
            item->objectData->updateBufferValue();
            item->isModified = false;
        }
        count++;
    }
    return workingGraph.getLeaves();
}

void SceneGraph::selectNextNode(){
    selectedNode = workingGraph.getNextTRMatrix();
}

TreeNode* SceneGraph::getSelectedNode(){
    if(selectedNode == nullptr){
        selectNextNode();
    }
    return selectedNode;
}

TreeNode* SceneGraph::findNextNode(TreeNode* currentNode){
    return nullptr;
}

void SceneGraph::updateNodes(){
    
}