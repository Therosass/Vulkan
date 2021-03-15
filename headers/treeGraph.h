#ifndef TREEGRAPH_H
#define TREEGRAPH_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <optional>
#include <iostream>

#include "renderable.h"

enum NODETYPES{
    TRANSFORMATION_NODE,
    GROUP_NODE,
    OBJECT_NODE
};

enum TRANSFORMATIONTYPE{
    ROTATION,
    SCALE,
    TRANSLATION
};

struct Transformation{
    enum TRANSFORMATIONTYPE type;
    glm::mat4 scalingMatrix;
    glm::mat4 translationMatrix;
    glm::mat4 rotationMatrix;

    Transformation operator*(Transformation transform){
        return transform;
    };
};

struct TreeNode{
    bool isModified;
    TreeNode* parentNode;
    std::vector<std::unique_ptr<TreeNode>> childNodes;
    enum NODETYPES nodeType;
    std::shared_ptr<Transformation> transformation;
    std::shared_ptr<Renderable> objectData;
    std::shared_ptr<TransformationMatrix> TRMatrix;

    void updateObject(TransformationMatrix* newMatrix){
        auto objectMatrix = TRMatrix;
        isModified = false;
        auto currentNode = this;
        while(currentNode->parentNode){
            currentNode = currentNode->parentNode;
            if(currentNode->nodeType == NODETYPES::TRANSFORMATION_NODE){
                newMatrix->rotate = newMatrix->rotate * currentNode->TRMatrix->rotate;
                newMatrix->translate = newMatrix->translate + currentNode->TRMatrix->translate;
                newMatrix->scale = newMatrix->scale * currentNode->TRMatrix->scale;
            }
        }
    }

    void updateChildren(Transformation* transform){
        
        if(nodeType == NODETYPES::OBJECT_NODE){
            *transformation = *transformation * (*transform);
        }
        for(auto& child : childNodes){

        }
        isModified = false;
        
    };

    void setUpdated(){
        for(auto& childNode : childNodes){
            if(childNode){
                childNode->setUpdated();
            }
        }
        isModified = true;
    }

    void deleteNode(){
        
        if(transformation != nullptr){
            transformation.reset();
        }
        if(objectData  != nullptr){
            objectData.reset();
        }
        for(auto& child : childNodes){
            child->deleteNode();
        }
        
    };

    TreeNode* findRoot(TreeNode* currentNode){
        while(currentNode->parentNode != nullptr){
            if(currentNode->parentNode->childNodes.size() > 1){
                
            }
            currentNode = currentNode->parentNode;
        }
        return currentNode;
    }
};

class TreeGraph{
public:
    TreeGraph();
    TreeNode* addNode(std::unique_ptr<TreeNode> childNode, TreeNode* parentNode);
    void update(TreeGraph& updateTarget);
    TreeNode* getRoot();

    const std::vector<TreeNode*>& getLeaves();
    const std::vector<TransformationMatrix*>& getObjectTRMatrices();
    TreeNode* getNextTRMatrix();
    TreeNode* findNextNode(TreeNode* currentNode = nullptr);

private:
    TreeNode* rootNode;
    std::vector<TreeNode*> leaves;
    std::vector<TreeNode*> AllTRMatrices;
    unsigned int currentMatrix = 0;
    std::vector<TransformationMatrix*> ObjectTRMatrixArray;
    
};

#endif