#ifndef TREEGRAPH_H
#define TREEGRAPH_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <optional>

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
    Transformation* transformation;
    Renderable* objectData;

    void updateChildren(Transformation* transform){
        
        if(nodeType == NODETYPES::OBJECT_NODE){
            *transformation = *transformation * (*transform);
        }
        for(auto& child : childNodes){

        }
        isModified = false;
        
    };

    void deleteNode(){
        
        if(transformation != nullptr){
            free(transformation);
        }
        if(objectData  != nullptr){
            free(objectData);
        }
        for(auto& child : childNodes){
            child->deleteNode();
        }
        
    };
};

class TreeGraph{
public:
    TreeGraph();
    void addNode(std::unique_ptr<TreeNode> childNode, TreeNode* parentNode = nullptr);
    void update(TreeGraph& updateTarget);
    
    std::vector<TreeNode*>& getLeaves();
private:
    std::unique_ptr<TreeNode> rootNode;
    std::vector<TreeNode*> leaves;

};

#endif