#include "treeGraph.h"
#include <memory>
#include <iostream>

TreeGraph::TreeGraph(){
    rootNode = std::make_unique<TreeNode>();
    std::cout << "TreeGraph created";
}

//  TreeGraph::addNode
//  Adds a new node to the parentNode, if no nodes are defined then the parent will be the root node
//  This function takes ownership of the parameter childNode.
//

// TODO: Node hierarchy

void TreeGraph::addNode(std::unique_ptr<TreeNode> childNode, TreeNode* parentNode){
    if(parentNode == nullptr){
        parentNode = rootNode.get();
        std::cout << "parent node is set to root node." << std::endl;
    }
    childNode->parentNode = parentNode;
    std::cout << "parent node is: " << parentNode << std::endl; 
    std::cout << "child nodes start on: " << rootNode->childNodes.data() << std::endl;
    parentNode->childNodes.push_back(std::move(childNode));
    auto newNode = parentNode->childNodes.back().get();
    if(newNode->nodeType == NODETYPES::OBJECT_NODE)
    {
        leaves.push_back(newNode);
    }
}

//  TreeGraph::update
//  Updates all nodes of the updateTarget, making the two trees contain identitical data
//

void TreeGraph::update(TreeGraph& updateTarget){

}

std::vector<TreeNode*>& TreeGraph::getLeaves(){
    return leaves;
}
