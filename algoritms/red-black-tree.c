#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

enum Color {
    RED,
    BLACK
};

typedef struct Data {
    unsigned int ID;
    char name [20];
    char surname [20];
} Data;

typedef struct Node {
    Data data;
    enum Color color;
    struct Node* parent;
    struct Node* leftNode;
    struct Node* rightNode;
} Node;

Node* InitNode(Data data,  enum Color color, Node* parent){
    Node* root = calloc(1, sizeof(Node));
    root->color = RED;
    root->data = data;
    root->color = color;
    root->parent = parent;
    return root;
}

void RotateLeft(Node* root){
    if (root->parent != NULL){
        Node* tmp = root->parent;
        root->parent = root->parent->parent;
        if (root->parent != NULL){
            if (root->parent->leftNode == tmp){
                root->parent->leftNode = root;
            } else if (root->parent->rightNode == tmp){
                root->parent->rightNode = root;
            }
        }
        tmp->rightNode = root->leftNode;
        root->leftNode = tmp;
        tmp->parent = root;
        root->color = BLACK;
        root->leftNode->color = RED;
    } else {
        printf("Critical ERROR while Left rotation: root->parent == NULL\n");
    }
}

void RotateRight(Node* root){
    if (root->parent != NULL){
        Node* tmp = root->parent;
        root->parent = root->parent->parent;
        if (root->parent != NULL){
            if (root->parent->leftNode == tmp){
                root->parent->leftNode = root;
            } else if (root->parent->rightNode == tmp){
                root->parent->rightNode = root;
            }
        }
        tmp->leftNode = root->rightNode;
        root->rightNode = tmp;
        tmp->parent = root;
        root->color = BLACK;
        root->rightNode->color = RED;
    } else {
        printf("Critical ERROR while Right rotation: root->parent == NULL\n");
    }
}

Node* RedBlackTreeBalance(Node* root) {
    // Node* current = root;
    while (root->parent != NULL){
        if (root->color == RED && root->parent->color == RED){
            if (root->parent->parent != NULL){
                if (root->parent->parent->leftNode == root->parent){ // we are in the left node in 2:
                    if (root->parent->parent->rightNode != NULL){
                        if (root->parent->parent->rightNode->color == RED){
                            root->parent->parent->color = RED;
                            root->parent->color = BLACK;
                            root->parent->parent->rightNode->color = BLACK;
                            continue;
                        }
                    }
                    if (root->parent->rightNode == root){
                        RotateLeft(root);
                        RotateRight(root);
                        continue;
                    }
                    if (root->parent->leftNode == root){
                        RotateRight(root->parent);
                        continue;
                    }
                }
                if (root->parent->parent->rightNode == root->parent){ // we are in the right node in 2:
                    if (root->parent->parent->leftNode != NULL){
                        if (root->parent->parent->leftNode->color == RED){
                            root->parent->parent->color = RED;
                            root->parent->color = BLACK;
                            root->parent->parent->leftNode->color = BLACK;
                            continue;
                        }
                    }
                    if (root->parent->leftNode == root){
                        RotateRight(root);
                        RotateLeft(root);
                        continue;
                    }
                    if (root->parent->rightNode == root){
                        RotateLeft(root->parent);
                        continue;
                    }
                }
            }
        }
        root = root->parent;
    }

    root->color = BLACK; // Function returns the root and it must always be black.
    return root;
}

Node* RedBlackTreeInsert (Node* root, Data data){
    if (root == NULL)
        return InitNode(data, BLACK, NULL);

    Node* current = root;
    while (current != NULL){
        if (current->data.ID > data.ID){
            if (current->leftNode != NULL){
                current = current->leftNode;
                continue;
            } else {
                current->leftNode = InitNode(data, RED, current);
                return RedBlackTreeBalance(current->leftNode);
            }
        } else if (current->data.ID < data.ID){
            if (current->rightNode != NULL){
                current = current->rightNode;
                continue;
            } else {
                current->rightNode = InitNode(data, RED, current);
                return RedBlackTreeBalance(current->rightNode);
            }
        }
    }
    return root;
}

Data* RedBlackTreeSearch (Node* root, unsigned int ID){
    while (root != NULL){
        if (root->data.ID == ID)
            return &(root->data);

        if (root->data.ID > ID){
            root = root->leftNode;
            continue;
        }
        if (root->data.ID < ID){
            root = root->rightNode;
            continue;
        }
    }
    return NULL;
}

int main(){
    Node* root = NULL;

    for (int i = 0; i <= 10; i++){
        Data data;
        data.ID = i;
        sprintf(data.name, "%d", i);
        sprintf(data.surname, "%d", i);

        root = RedBlackTreeInsert(root, data);
    }

    // Data* data = RedBlackTreeSearch(root, 10);
    return 0;
}
