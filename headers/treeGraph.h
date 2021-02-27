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
    std::shared_ptr<Transformation> transformation;
    std::shared_ptr<Renderable> objectData;
    std::shared_ptr<TransformationMatrix> TRMatrix;

    void updateObject(TransformationMatrix* newMatrix){
        TRMatrix->rotate = newMatrix->rotate;
        TRMatrix->translate = newMatrix->translate;
        TRMatrix->scale = newMatrix->scale;
    }

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
            transformation.reset();
        }
        if(objectData  != nullptr){
            objectData.reset();
        }
        for(auto& child : childNodes){
            child->deleteNode();
        }
        
    };
};

class TreeGraph{
public:
    TreeGraph();
    TreeNode* addNode(std::unique_ptr<TreeNode> childNode, TreeNode* parentNode);
    void update(TreeGraph& updateTarget);
    TreeNode* getRoot();

    const std::vector<TreeNode*>& getLeaves();
    const std::vector<TransformationMatrix*>& getMatrices();

private:
    TreeNode* rootNode;
    std::vector<TreeNode*> leaves;
    std::vector<TransformationMatrix*> TRMatrixArray;

};

#endif