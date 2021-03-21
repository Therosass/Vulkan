#include "treeGraph.h"
#include <memory>
#include <iostream>

TreeGraph::TreeGraph(){
    rootNode = new TreeNode();
    rootNode->nodeType = NODETYPES::GROUP_NODE;
    rootNode->parentNode = nullptr;
    std::cout << "TreeGraph created";
}

//  TreeGraph::addNode
//  Adds a new node to the parentNode, if no nodes are defined then the parent will be the root node
//  This function takes ownership of the parameter childNode.
//

// TODO: Node hierarchy

TreeNode* TreeGraph::addNode(std::unique_ptr<TreeNode> childNode, TreeNode* parentNode){
    if(parentNode == nullptr && childNode->nodeType == NODETYPES::TRANSFORMATION_NODE)
    {
        parentNode = rootNode;
        std::cout << "parent node is set to root node." << std::endl;
    }
    else if(parentNode == nullptr){
        std::runtime_error("You done goofed!");
    }
    if(childNode->nodeType == NODETYPES::OBJECT_NODE)
    {
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
        childNode->TRMatrix = newTransform;
    }
    childNode->parentNode = parentNode;
    std::cout << "parent node is: " << parentNode << std::endl; 
    std::cout << "child nodes start on: " << rootNode->childNodes.data() << std::endl;
    parentNode->childNodes.push_back(std::move(childNode));
    auto newNode = parentNode->childNodes.back().get();


    // TODO: This is horrible with ownership, please fix me
    if(newNode->nodeType == NODETYPES::TRANSFORMATION_NODE){
        AllTRMatrices.push_back(newNode);
    }
    if(newNode->nodeType == NODETYPES::OBJECT_NODE)
    {
        leaves.push_back(newNode);
        ObjectTRMatrixArray.push_back(newNode->TRMatrix.get());
    }
    return newNode;
}

//  TreeGraph::update
//  Updates all nodes of the updateTarget, making the two trees contain identitical data
//

void TreeGraph::update(TreeGraph& updateTarget){

}

TreeNode* TreeGraph::getRoot(){
    return rootNode;
}


const std::vector<TreeNode*>& TreeGraph::getLeaves(){
    return leaves;
}

const std::vector<TransformationMatrix*>& TreeGraph::getObjectTRMatrices(){
    return ObjectTRMatrixArray;
}

// TODO: Add proper error handling

TreeNode* TreeGraph::getNextTRMatrix(){
    if(AllTRMatrices.size() == 0){
        return nullptr;
    }
    if(currentMatrix < AllTRMatrices.size() -1){
        currentMatrix++;   
    }
    else{
        currentMatrix = 0;
    }    
    return AllTRMatrices[currentMatrix];
}
