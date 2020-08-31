#include "treeGraph.h"

//  TreeGraph::addNode
//  Adds a new node to the parentNode, if no nodes are defined then the parent will be the root node
//

void TreeGraph::addNode(TreeNode* childNode, TreeNode* parentNode){
    if(parentNode == nullptr){
        parentNode = &rootNode;
    }
    parentNode->childNodes.push_back(childNode);
}

//  TreeGraph::update
//  Updates all nodes of the updateTarget, making the two trees contain identitical data
//

void TreeGraph::update(TreeGraph& updateTarget){

}