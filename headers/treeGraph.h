#ifndef TREEGRAPH_H
#define TREEGRAPH_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

    };
};

struct TreeNode{
    std::vector<TreeNode*> childNodes;
    bool isModified;
    TreeNode* parentNode;
    enum NODETYPES nodeType;
    Transformation* transformation;
    Renderable* objectData;

    void updateChildren(Transformation* transform){
        if(nodeType == NODETYPES::OBJECT_NODE){
            *transformation = *transformation * (*transform);
        }
        for(auto child : childNodes){
            
        }
        isModified = false;
    };

    void deleteNode(){
        if(transformation != nullptr){
            free(transformation);
        }
        if(objectData != nullptr){
            free(objectData);
        }
        for(auto child : childNodes){
            child->deleteNode();
        }
    };
};

class TreeGraph{
public:
    void addNode(TreeNode* childNode, TreeNode* parentNode = nullptr);
    void update(TreeGraph& updateTarget);
private:
    TreeNode rootNode;
};

#endif