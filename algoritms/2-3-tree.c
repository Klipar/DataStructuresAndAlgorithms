#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct Data {
    unsigned int ID;
    char name [20];
    char surname [20];
} Data;

typedef struct tree_2_3 {
    Data leftData;
    Data rightData;
    bool isRightExist;
    struct tree_2_3 *leftNode;
    struct tree_2_3 *middleNode;
    struct tree_2_3 *rightNode;
} tree_2_3;

tree_2_3* initNode(Data data){
    tree_2_3* node = (tree_2_3*)calloc(1, sizeof(tree_2_3));
    node->leftData = data;
    return node;
}

void ObsidianVisualization(tree_2_3 *root, const char *dirName){
    if (root == NULL)
        return;

    char path [100] = {};
    if (root->isRightExist)
        sprintf(path + strlen(path), "%s/%d-%d.md", dirName, root->leftData.ID, root->rightData.ID);
    else
        sprintf(path + strlen(path), "%s/%d.md", dirName, root->leftData.ID, root->rightData.ID);

    FILE *file = fopen(path, "w");
    if (file == NULL) {
        perror("Failed to open file!!!");
        return;
    }
    if (root->isRightExist)
        fprintf(file, "# %d | %d\n", root->leftData.ID, root->rightData.ID);
    else {
        fprintf(file, "# %d\n", root->leftData.ID);
    }
    if (root->leftNode != NULL){
        if (root->leftNode->isRightExist)
            fprintf(file, "Left node: [[%d-%d.md]]\n", root->leftNode->leftData.ID, root->leftNode->rightData.ID);
        else
            fprintf(file, "Left node: [[%d.md]]\n", root->leftNode->leftData.ID);

        ObsidianVisualization(root->leftNode, "TreeVisualization");
    }
    if (root->middleNode != NULL){
        if (root->middleNode->isRightExist)
            fprintf(file, "Middle node: [[%d-%d.md]]\n", root->middleNode->leftData.ID, root->middleNode->rightData.ID);
        else
            fprintf(file, "Middle node: [[%d.md]]\n", root->middleNode->leftData.ID);

        ObsidianVisualization(root->middleNode, "TreeVisualization");
    }
    if (root->rightNode != NULL && root->isRightExist){
        if (root->rightNode->isRightExist)
            fprintf(file, "Right node: [[%d-%d.md]]\n", root->rightNode->leftData.ID, root->rightNode->rightData.ID);
        else
            fprintf(file, "Right node: [[%d.md]]\n", root->rightNode->leftData.ID);

        ObsidianVisualization(root->rightNode, "TreeVisualization");
    }
    fclose(file);
}

tree_2_3* Tree_2_3_Insert(tree_2_3 *root, Data data){
    if (!root)
        return initNode(data);
    tree_2_3 *tmp;
    if (!root->leftNode && !root->middleNode){
        if (!root->isRightExist){
            if (root->leftData.ID > data.ID){
                root->rightData = root->leftData;
                root->leftData = data;
            }else
                root->rightData = data;

            root->isRightExist = true;
            return root;
        } else {
            if (root->leftData.ID > data.ID){           //data.ID < root->leftData.ID < root->rightData.ID
                tmp = initNode(root->leftData);
                tmp->leftNode = initNode(data);
                tmp->middleNode = root;
                root->leftData = root->rightData;
            } else if (root->rightData.ID < data.ID){   // root->leftData.ID < root->rightData.ID <data.ID
                tmp = initNode(root->rightData);
                tmp->leftNode = root;
                tmp->middleNode = initNode(data);
            } else {                                // root->leftData.ID <data.ID < root->rightData.ID
                tmp = initNode(data);
                tmp->leftNode = root;
                tmp->middleNode = initNode(root->rightData);
            }
            root->isRightExist = false;
        }
        return tmp;
    }else if (data.ID < root->leftData.ID){
        tmp = Tree_2_3_Insert(root->leftNode, data);

        if (root->leftNode == tmp){
            return root;
        } else {
            if (!tmp->isRightExist){
                if (!root->isRightExist){
                    root->isRightExist = true;
                    root->rightData = root->leftData;
                    root->leftData = tmp->leftData;
                    root->rightNode = root->middleNode;
                    root->leftNode = tmp->leftNode;
                    root->middleNode = tmp->middleNode;
                    free (tmp);
                    return root;
                }
            } else {
                tree_2_3 *newRoot = initNode(root->leftData);
                newRoot->leftNode = tmp;
                newRoot->middleNode = root;
                root->leftData = root->rightData;
                root->isRightExist = false;
                root->leftNode = root->middleNode;
                root->middleNode = root->rightNode;
                root->rightNode = NULL;
                return newRoot;
            }
        }

    } else if (data.ID >= root->leftData.ID && !root->isRightExist){
        tmp = Tree_2_3_Insert(root->middleNode, data);

        if (root->middleNode == tmp) {
            return root;

        } else {
            root->isRightExist = true;
            root->rightData = tmp->leftData;
            root->middleNode = tmp->leftNode;
            root->rightNode = tmp->middleNode;
            free (tmp);
            return root;
        }

    } else if (data.ID > root->rightData.ID && root->isRightExist){
        tmp = Tree_2_3_Insert(root->rightNode, data);

        if (root->rightNode == tmp) {
            return root;

        } else {
            tree_2_3 *newRoot = initNode(root->rightData);
            newRoot->leftNode = root;
            root->isRightExist = false;
            root->rightNode = NULL;
            newRoot->middleNode = tmp;
            return newRoot;
        }
    } else { //data.ID in the midle
        tmp = Tree_2_3_Insert(root->middleNode, data);
        if (root->middleNode == tmp){
            return root;
        }else {
            tree_2_3 *newRoot = initNode(tmp->leftData);
            newRoot->middleNode = initNode(root->rightData);
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

tree_2_3* Tree_2_3_Search (tree_2_3 *root, int ID){
    if (root == NULL) return NULL;

    if (root->leftData.ID == ID) return root;
    if (root->rightData.ID == ID && root->isRightExist) return root;

    if (root->leftData.ID > ID)
        return Tree_2_3_Search(root->leftNode, ID);

    if (root->rightData.ID < ID && root->isRightExist)
        return Tree_2_3_Search(root->rightNode, ID);

    if (root->leftData.ID < ID)
        return Tree_2_3_Search(root->middleNode, ID);
}

Data getMaxData (tree_2_3 *root){ //if root is NULL, an error will occur. It is handled before calling the function
    if (root->rightNode)
        return getMaxData(root->rightNode);
    if (root->middleNode)
        return getMaxData(root->middleNode);
    if (root->isRightExist)
        return root->rightData;
    return root->leftData;
}

tree_2_3* Tree_2_3_Delete (tree_2_3 *root, int ID){
    if (root == NULL) return NULL;
    tree_2_3 *tmp;

    if (!root->leftNode && !root->middleNode){
        if (!root->isRightExist){
            if (root->leftData.ID == ID){
                free (root);
                return NULL;
            } else {
                return root;
            }

        } else {
            if (root->rightData.ID == ID){
                root->isRightExist = false;
                return root;
            } else if (root->leftData.ID == ID){
                root->leftData = root->rightData;
                root->isRightExist = false;
                return root;
            } else {
                return root;
            }
        }
    } else {
        if ( ID < root->leftData.ID){
            tmp = Tree_2_3_Delete(root->leftNode, ID);
            if (tmp == root->leftNode){
                return root;

            }else if (!root->isRightExist){ // if its only onedata.ID in root node
                if (tmp == NULL){
                    if (root->middleNode->isRightExist){
                        root->leftNode = initNode(root->leftData);
                        root->leftData = root->middleNode->leftData;
                        root->middleNode->leftData = root->middleNode->rightData;
                        root->middleNode->isRightExist = false;
                        return root;
                    } else {
                        root->middleNode->isRightExist = true;
                        root->middleNode->rightData = root->middleNode->leftData;
                        root->middleNode->leftData = root->leftData;
                        tmp = root->middleNode;
                        free(root);
                        return tmp;
                    }

                } else{
                    if (root->middleNode->isRightExist){
                        root->leftNode = initNode(root->leftData);
                        root->leftData = root->middleNode->leftData;
                        root->middleNode->leftData = root->middleNode->rightData;
                        root->middleNode->isRightExist = false;
                        root->leftNode->leftNode = tmp;
                        root->leftNode->middleNode = root->middleNode->leftNode;
                        root->middleNode->leftNode = root->middleNode->middleNode;
                        root->middleNode->middleNode = root->middleNode->rightNode;
                        return root;
                    } else {
                        root->middleNode->isRightExist = true;
                        root->middleNode->rightData = root->middleNode->leftData;
                        root->middleNode->leftData = root->leftData;
                        root->middleNode->rightNode = root->middleNode->middleNode;
                        root->middleNode->middleNode = root->middleNode->leftNode;
                        root->middleNode->leftNode = tmp;
                        tmp = root->middleNode;
                        free(root);
                        return tmp;
                    }
                }
            }else if (root->isRightExist){ // if in root 2data.ID
                if (tmp == NULL){ // links are not taken into account because NULL can be returned only when the leaf is deleted
                    if (!root->middleNode->isRightExist){
                        root->leftNode = root->middleNode;
                        root->middleNode = root->rightNode;
                        root->leftNode->isRightExist = true;
                        root->leftNode->rightData = root->leftNode->leftData;
                        root->leftNode->leftData = root->leftData;
                        root->leftData = root->rightData;
                        root->isRightExist = false;
                        return root;
                    } else if (root->middleNode->isRightExist){
                        root->leftNode = initNode(root->leftData);
                        root->leftData = root->middleNode->leftData;
                        root->middleNode->leftData = root->middleNode->rightData;
                        root->middleNode->isRightExist = false;
                        return root;
                    }
                } else{
                    if (!root->middleNode->isRightExist){
                        root->middleNode->isRightExist = true;
                        root->middleNode->rightData = root->middleNode->leftData;
                        root->middleNode->leftData = root->leftData;
                        root->middleNode->rightNode = root->middleNode->middleNode;
                        root->middleNode->middleNode = root->middleNode->leftNode;
                        root->middleNode->leftNode = tmp;
                        root->leftNode = root->middleNode;
                        root->middleNode = root->rightNode;
                        root->leftData = root->rightData;
                        root->isRightExist = false;
                        return root;

                    } else if (root->middleNode->isRightExist){
                        root->leftNode = initNode(root->leftData);
                        root->leftData = root->middleNode->leftData;
                        root->middleNode->leftData = root->middleNode->rightData;
                        root->leftNode = tmp;
                        root->leftNode->middleNode = root->middleNode->leftNode;
                        root->middleNode->leftNode = root->middleNode->middleNode;
                        root->middleNode->middleNode = root->middleNode->rightNode;
                        root->middleNode->isRightExist = false;
                        return root;
                    }
                }
            }
        } else if (root->isRightExist && root->rightData.ID < ID){
            tmp = Tree_2_3_Delete(root->rightNode, ID);
            if (tmp == root->rightNode){
                return root;

            }if (tmp == NULL){
                if (root->middleNode->isRightExist){
                    root->rightNode = initNode(root->rightData);
                    root->rightData = root->middleNode->rightData;
                    root->middleNode->isRightExist = false;
                    return root;
                } else{
                    root->middleNode->isRightExist = true;
                    root->middleNode->rightData = root->rightData;
                    root->isRightExist = false;
                    return root;
                }
            } else{
                if (!root->middleNode->isRightExist){
                    root->middleNode->isRightExist = true;
                    root->middleNode->rightData = root->rightData;
                    root->isRightExist = false;
                    root->middleNode->rightNode = tmp;
                    return root;
                } else if (root->middleNode->isRightExist){
                    root->rightNode = initNode(root->rightData);
                    root->rightData = root->middleNode->rightData;
                    root->rightNode->middleNode = tmp;
                    root->rightNode->leftNode = root->middleNode->rightNode;
                    root->middleNode->isRightExist = false;
                    return root;
                }
            }

        } else if (root->isRightExist && root->rightData.ID > ID && root->leftData.ID < ID || !root->isRightExist && root->leftData.ID < ID){
            tmp = Tree_2_3_Delete(root->middleNode, ID);

            if (tmp == root->middleNode){
                return root;

            } else if (!root->isRightExist){
                if (tmp == NULL){
                    if (!root->leftNode->isRightExist){
                        root->leftNode->isRightExist = true;
                        root->leftNode->rightData = root->leftData;
                        tmp = root->leftNode;
                        free (root);
                        return tmp;
                    } else if (root->leftNode->isRightExist){
                        root->middleNode = initNode(root->leftData);
                        root->leftData = root->leftNode->rightData;
                        root->leftNode->isRightExist = false;
                        return root;
                    }
                } else {
                    if (!root->leftNode->isRightExist){
                        root->leftNode->isRightExist = true;
                        root->leftNode->rightData = root->leftData;
                        root->leftNode->rightNode = tmp;
                        tmp = root->leftNode;
                        free (root);
                        return tmp;
                    } else if (root->leftNode->isRightExist){
                        root->middleNode = initNode(root->leftData);
                        root->leftData = root->leftNode->rightData;
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
                        root->leftNode->rightData = root->leftData;
                        root->leftData = root->rightData;
                        root->middleNode = root->rightNode;
                        return root;
                    } else {
                        root->middleNode = initNode(root->leftData);
                        root->leftData = root->leftNode->rightData;
                        root->leftNode->isRightExist = false;
                        return root;
                    }
                } else {
                    if (!root->leftNode->isRightExist){
                        root->leftNode->isRightExist = true;
                        root->leftNode->rightData = root->leftData;
                        root->leftNode->rightNode = tmp;
                        root->leftData = root->rightData;
                        root->isRightExist = false;
                        return root;
                    } else if (root->leftNode->isRightExist){
                        root->middleNode = initNode(root->leftData);
                        root->leftData = root->leftNode->rightData;
                        root->middleNode->leftNode = root->leftNode->rightNode;
                        root->middleNode->middleNode = tmp;
                        root->leftNode->isRightExist = false;
                        return root;
                    }
                }
            }

        } else if (root->rightData.ID == ID && root->isRightExist){
            Data data = getMaxData(root->middleNode);
            tmp = Tree_2_3_Delete(root, data.ID);
            tmp->rightData = data;
            return tmp;
        } if (root->leftData.ID == ID){
            Data data = getMaxData(root->leftNode);
            tmp = Tree_2_3_Delete(root, data.ID);
            tmp->leftData = data;
            return tmp;
        }else {
            return root;    // item does not exist
        }
    }
}

int main(){
    tree_2_3 *root = NULL;
    for (int i = 0; i <= 50; i++){
        Data data;
        data.ID = i;
        sprintf(data.name, "%d", i);
        sprintf(data.surname, "%d", i);

        root = Tree_2_3_Insert(root, data);
    }
    ObsidianVisualization(root, "TreeVisualization");
    for (int i = 50; i >=0; i--)
        root = Tree_2_3_Delete(root, i);

    Tree_2_3_Free(root);
    return 0;
}
