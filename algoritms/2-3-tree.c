#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct tree_2_3 {
    int leftValue;
    int rightValue;
    bool isRightExist;
    struct tree_2_3 *leftNode;
    struct tree_2_3 *middleNode;
    struct tree_2_3 *rightNode;
} tree_2_3;

tree_2_3* initNode(int value){
    tree_2_3* node = (tree_2_3*)calloc(1, sizeof(tree_2_3));
    node->leftValue = value;
    return node;
}

tree_2_3* Tree_2_3_Insert(tree_2_3 *root, int value){
    if (!root)
        return initNode(value);
    tree_2_3 *tmp;
    if (!root->leftNode && !root->middleNode && !root->rightNode){
        if (!root->isRightExist){
            if (root->leftValue > value){
                root->rightValue = root->leftValue;
                root->leftValue = value;
            }else
                root->rightValue = value;

            root->isRightExist = true;
            return root;
        } else {
            if (root->leftValue > value){           // value < root->leftValue < root->rightValue
                tmp = initNode(root->leftValue);
                tmp->leftNode = initNode(value);
                tmp->middleNode = root;
                root->leftValue = root->rightValue;
            } else if (root->rightValue < value){   // root->leftValue < root->rightValue < value
                tmp = initNode(root->rightValue);
                tmp->leftNode = root;
                tmp->middleNode = initNode(value);
            } else {                                // root->leftValue < value < root->rightValue
                tmp = initNode(value);
                tmp->leftNode = root;
                tmp->middleNode = initNode(root->rightValue);
            }
            root->isRightExist = false;
        }
        return tmp;
    }else if (value < root->leftValue){
        tmp = Tree_2_3_Insert(root->leftNode, value);

        if (root->leftNode == tmp){
            return root;
        } else {
            if (!tmp->isRightExist){
                if (!root->isRightExist){
                    root->isRightExist = true;
                    root->rightValue = root->leftValue;
                    root->leftValue = tmp->leftValue;
                    root->rightNode = root->middleNode;
                    root->leftNode = tmp->leftNode;
                    root->middleNode = tmp->middleNode;
                    free (tmp);
                    return root;
                }
            } else {
                tree_2_3 *newRoot = initNode(root->leftValue);
                newRoot->leftNode = tmp;
                newRoot->middleNode = root;
                root->leftValue = root->rightValue;
                root->isRightExist = false;
                root->leftNode = root->middleNode;
                root->middleNode = root->rightNode;
                root->rightNode = NULL;
                return newRoot;
            }
        }

    } else if (value >= root->leftValue && !root->isRightExist){
        tmp = Tree_2_3_Insert(root->middleNode, value);

        if (root->middleNode == tmp) {
            return root;

        } else {
            root->isRightExist = true;
            root->rightValue = tmp->leftValue;
            root->middleNode = tmp->leftNode;
            root->rightNode = tmp->middleNode;
            free (tmp);
            return root;
        }

    } else if (value > root->rightValue && root->isRightExist){
        tmp = Tree_2_3_Insert(root->rightNode, value);

        if (root->rightNode == tmp) {
            return root;

        } else {
            tree_2_3 *newRoot = initNode(root->rightValue);
            newRoot->leftNode = root;
            root->isRightExist = false;
            root->rightNode = NULL;
            newRoot->middleNode = tmp;
            return newRoot;
        }
    } else { // value in the midle
        tmp = Tree_2_3_Insert(root->middleNode, value);
        if (root->middleNode == tmp){
            return root;
        }else {
            tree_2_3 *newRoot = initNode(tmp->leftValue);
            newRoot->middleNode = initNode(root->rightValue);
            newRoot->middleNode->leftNode = tmp->middleNode;
            newRoot->middleNode->middleNode = root->rightNode;
            newRoot->leftNode = root;
            root->isRightExist = false;
            root->rightNode = NULL;
            newRoot->leftNode->middleNode = tmp->leftNode;
            free (tmp);
            return newRoot;
        }
    }

    return root;
}

void Tree_2_3_Free(tree_2_3 *root){
    if (!root)
        return;

    Tree_2_3_Free(root->leftNode);
    Tree_2_3_Free(root->middleNode);
    Tree_2_3_Free(root->rightNode);

    free(root);
    }

int main(){
    tree_2_3 *root = NULL;
    for (int i = -4; i <= 10; i++)
        root = Tree_2_3_Insert(root, i);

    Tree_2_3_Free(root);
    return 0;
}
