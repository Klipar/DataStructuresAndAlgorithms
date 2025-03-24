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

tree_2_3 Tree_2_3_Search (tree_2_3 *root, int value){
    if (root == NULL) return NULL;

    if (root->leftValue == value) return root;
    if (root->rightValue == value && root->isRightExist) return root;

    if (root->leftValue > value)
        return Tree_2_3_Search(root->leftNode, value);

    if (root->rightValue < value && root->isRightExist)
        return Tree_2_3_Search(root->rightNode, value);

    if (root->leftValue < value)
        return Tree_2_3_Search(root->middleNode, value);
}

int getMaxValue (tree_2_3 *root){ //if root is NULL, an error will occur. It is handled before calling the function
    if (root->rightNode)
        return getMaxValue(root->rightNode);
    if (root->middleNode)
        return getMaxValue(root->middleNode);
    if (root->isRightExist)
        return root->rightValue;
    return root->leftValue;
}

tree_2_3* Tree_2_3_Delete (tree_2_3 *root, int value){
    if (root == NULL) return NULL;
    tree_2_3 *tmp;

    if (!root->leftNode && !root->middleNode){
        if (!root->isRightExist){
            if (root->leftValue == value){
                free (root);
                return NULL;
            } else {
                return root;
            }

        } else {
            if (root->rightValue == value){
                root->isRightExist = false;
                return root;
            } else if (root->leftValue == value){
                root->leftValue = root->rightValue;
                root->isRightExist = false;
                return root;
            } else {
                return root;
            }
        }
    } else {
        if (value < root->leftValue){
            tmp = Tree_2_3_Delete(root->leftNode, value);
            if (tmp == root->leftNode){
                return root;

            }else if (!root->isRightExist){ // if its only one value in root node
                if (tmp == NULL){
                    if (root->middleNode->isRightExist){
                        root->leftNode = initNode(root->leftValue);
                        root->leftValue = root->middleNode->leftValue;
                        root->middleNode->leftValue = root->middleNode->rightValue;
                        root->middleNode->isRightExist = false;
                        return root;
                    } else {
                        root->middleNode->isRightExist = true;
                        root->middleNode->rightValue = root->middleNode->leftValue;
                        root->middleNode->leftValue = root->leftValue;
                        tmp = root->middleNode;
                        free(root);
                        return tmp;
                    }

                } else{
                    if (root->middleNode->isRightExist){
                        root->leftNode = initNode(root->leftValue);
                        root->leftValue = root->middleNode->leftValue;
                        root->middleNode->leftValue = root->middleNode->rightValue;
                        root->middleNode->isRightExist = false;
                        root->leftNode->leftNode = tmp;
                        root->leftNode->middleNode = root->middleNode->leftNode;
                        root->middleNode->leftNode = root->middleNode->middleNode;
                        root->middleNode->middleNode = root->middleNode->rightNode;
                        return root;
                    } else {
                        root->middleNode->isRightExist = true;
                        root->middleNode->rightValue = root->middleNode->leftValue;
                        root->middleNode->leftValue = root->leftValue;
                        root->middleNode->rightNode = root->middleNode->middleNode;
                        root->middleNode->middleNode = root->middleNode->leftNode;
                        root->middleNode->leftNode = tmp;
                        tmp = root->middleNode;
                        free(root);
                        return tmp;
                    }
                }
            }else if (root->isRightExist){ // if in root 2 value
                if (tmp == NULL){ // links are not taken into account because NULL can be returned only when the leaf is deleted
                    if (!root->middleNode->isRightExist){
                        root->leftNode = root->middleNode;
                        root->middleNode = root->rightNode;
                        root->leftNode->isRightExist = true;
                        root->leftNode->rightValue = root->leftNode->leftValue;
                        root->leftNode->leftValue = root->leftValue;
                        root->leftValue = root->rightValue;
                        root->isRightExist = false;
                        return root;
                    } else if (root->middleNode->isRightExist){
                        root->leftNode = initNode(root->leftValue);
                        root->leftValue = root->middleNode->leftValue;
                        root->middleNode->leftValue = root->middleNode->rightValue;
                        root->middleNode->isRightExist = false;
                        return root;
                    }
                } else{
                    if (!root->middleNode->isRightExist){
                        root->middleNode->isRightExist = true;
                        root->middleNode->rightValue = root->middleNode->leftValue;
                        root->middleNode->leftValue = root->leftValue;
                        root->middleNode->rightNode = root->middleNode->middleNode;
                        root->middleNode->middleNode = root->middleNode->leftNode;
                        root->middleNode->leftNode = tmp;
                        root->leftNode = root->middleNode;
                        root->middleNode = root->rightNode;
                        root->leftValue = root->rightValue;
                        root->isRightExist = false;
                        return root;

                    } else if (root->middleNode->isRightExist){
                        root->leftNode = initNode(root->leftValue);
                        root->leftValue = root->middleNode->leftValue;
                        root->middleNode->leftValue = root->middleNode->rightValue;
                        root->leftNode = tmp;
                        root->leftNode->middleNode = root->middleNode->leftNode;
                        root->middleNode->leftNode = root->middleNode->middleNode;
                        root->middleNode->middleNode = root->middleNode->rightNode;
                        root->middleNode->isRightExist = false;
                        return root;
                    }
                }
            }
        } else if (root->isRightExist && root->rightValue < value){
            tmp = Tree_2_3_Delete(root->rightNode, value);
            if (tmp == root->rightNode){
                return root;

            }if (tmp == NULL){
                if (root->middleNode->isRightExist){
                    root->rightNode = initNode(root->rightValue);
                    root->rightValue = root->middleNode->rightValue;
                    root->middleNode->isRightExist = false;
                    return root;
                } else{
                    root->middleNode->isRightExist = true;
                    root->middleNode->rightValue = root->rightValue;
                    root->isRightExist = false;
                    return root;
                }
            } else{
                if (!root->middleNode->isRightExist){
                    root->middleNode->isRightExist = true;
                    root->middleNode->rightValue = root->rightValue;
                    root->isRightExist = false;
                    root->middleNode->rightNode = tmp;
                    return root;
                } else if (root->middleNode->isRightExist){
                    root->rightNode = initNode(root->rightValue);
                    root->rightValue = root->middleNode->rightValue;
                    root->rightNode->middleNode = tmp;
                    root->rightNode->leftNode = root->middleNode->rightNode;
                    root->middleNode->isRightExist = false;
                    return root;
                }
            }

        } else if (root->isRightExist && root->rightValue > value && root->leftValue < value || !root->isRightExist && root->leftValue < value){
            tmp = Tree_2_3_Delete(root->middleNode, value);

            if (tmp == root->middleNode){
                return root;

            } else if (!root->isRightExist){
                if (tmp == NULL){
                    if (!root->leftNode->isRightExist){
                        root->leftNode->isRightExist = true;
                        root->leftNode->rightValue = root->leftValue;
                        tmp = root->leftNode;
                        free (root);
                        return tmp;
                    } else if (root->leftNode->isRightExist){
                        root->middleNode = initNode(root->leftValue);
                        root->leftValue = root->leftNode->rightValue;
                        root->leftNode->isRightExist = false;
                        return root;
                    }
                } else {
                    if (!root->leftNode->isRightExist){
                        root->leftNode->isRightExist = true;
                        root->leftNode->rightValue = root->leftValue;
                        root->leftNode->rightNode = tmp;
                        tmp = root->leftNode;
                        free (root);
                        return tmp;
                    } else if (root->leftNode->isRightExist){
                        root->middleNode = initNode(root->leftValue);
                        root->leftValue = root->leftNode->rightValue;
                        root->leftNode->isRightExist = false;
                        root->middleNode->middleNode = tmp;
                        root->middleNode->leftNode = root->leftNode->rightNode;
                        return root;
                    }
                }
            } else if (root->isRightExist){
                if (tmp == NULL){
                    if (!root->leftNode->isRightExist){
                        root->leftNode->isRightExist = true;
                        root->leftNode->rightValue = root->leftValue;
                        root->leftValue = root->rightValue;
                        root->middleNode = root->rightNode;
                        return root;
                    } else {
                        root->middleNode = initNode(root->leftValue);
                        root->leftValue = root->leftNode->rightValue;
                        root->leftNode->isRightExist = false;
                        return root;
                    }
                } else {
                    if (!root->leftNode->isRightExist){
                        root->leftNode->isRightExist = true;
                        root->leftNode->rightValue = root->leftValue;
                        root->leftNode->rightNode = tmp;
                        root->leftValue = root->rightValue;
                        root->isRightExist = false;
                        return root;
                    } else if (root->leftNode->isRightExist){
                        root->middleNode = initNode(root->leftValue);
                        root->leftValue = root->leftNode->rightValue;
                        root->middleNode->leftNode = root->leftNode->rightNode;
                        root->middleNode->middleNode = tmp;
                        root->leftNode->isRightExist = false;
                        return root;
                    }
                }
            }

        } else if (root->rightValue == value && root->isRightExist){
            root->rightValue = getMaxValue(root->middleNode);
            return Tree_2_3_Delete(root, root->rightValue);

        } if (root->leftValue == value){
            root->leftValue = getMaxValue(root->leftNode);
            return Tree_2_3_Delete(root, root->leftValue);
        }else {
            return root;    // item does not exist
        }
    }
}

int main(){
    tree_2_3 *root = NULL;
    for (int i = -4; i <= 10; i++)
        root = Tree_2_3_Insert(root, i);

    Tree_2_3_Free(root);
    return 0;
}
