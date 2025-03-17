#include <stdlib.h>
#include <stdio.h>

typedef struct tree_2_3 {
    int leftValue;
    int rightValue;
    char isRightExist;
    struct tree_2_3 *leftNode;
    struct tree_2_3 *midleNode;
    struct tree_2_3 *rightNode;
} tree_2_3;

tree_2_3* initNode(int value){
    tree_2_3* node = (tree_2_3*)calloc(1, sizeof(tree_2_3));
    node->leftValue = value;
    node->isRightExist = 0;
    return node;
}

tree_2_3* Tree_2_3_Add(tree_2_3 *root, int value){
    if (!root)
        return initNode(value);

    if (!root->leftNode && !root->midleNode && !root->rightNode){
        if (!root->isRightExist){
            if (root->leftValue > value){
                root->rightValue = root->leftValue;
                root->leftValue = value;
            }else if (root->rightValue < value){ //delete this condition in case allowed duplication
                root->rightValue = value;
            }
            root->isRightExist = 1;
        } else {
            if (root->leftValue > value){
                root->leftNode = initNode(value);
                root->midleNode = initNode(root->rightValue);
            } else if (root->rightValue < value){
                root->leftNode = initNode(root->leftValue);
                root->midleNode = initNode(value);
                root->leftValue = root->rightValue;
                root->rightValue = root->isRightExist = 0;
            } else {
                root->leftNode = initNode(root->leftValue);
                root->midleNode = initNode(root->rightValue);
                root->leftValue = value;
            }
            root->rightValue = root->isRightExist = 0;
        }
        return root;
    }



    return root;
}

void Tree_2_3_Free(tree_2_3 *root){
    if (!root)
        return;

    Tree_2_3_Free(root->leftNode);
    Tree_2_3_Free(root->midleNode);
    Tree_2_3_Free(root->rightNode);

    free(root);
    }

int main(){
    tree_2_3 *root = Tree_2_3_Add(NULL, 5);

    Tree_2_3_Free(root);
    return 0;
}
