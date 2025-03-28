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

Node* RotateLeft(Node* root){
    if (root->parent != NULL){
        Node* tmp = root->parent;
        root->parent = root->parent->parent;
        tmp->rightNode = root->leftNode;
        root->leftNode = tmp;
        tmp->parent = root;
        root->color = BLACK;
        root->leftNode->color = RED;
    }
}

Node* RotateRight(Node* root){
    if (root->parent != NULL){
        Node* tmp = root->parent;
        root->parent = root->parent->parent;
        tmp->leftNode = root->rightNode;
        root->rightNode = tmp;
        tmp->parent = root;
        root->color = BLACK;
        root->rightNode->color = RED;
    }
}

Node* RedBlackTreeInsert (Node* root, Data data){
    if (root == NULL)
        return InitNode(data, BLACK, NULL);

    if (root->data.ID > data.ID){           // go tu left
        if (root->leftNode != NULL)
            RedBlackTreeInsert(root->leftNode, data);
        else
            root->leftNode = InitNode(data, RED, root);


        if (root->parent == NULL && root->color == RED){ // red root //1
            root->color = BLACK;
            return root;
        }
        if (root->leftNode != NULL){
            if (root->leftNode->color == RED && root->color == RED){    // double RED needs to repair
                if (root->parent != NULL){
                    if (root->parent->leftNode == root){
                        if (root->parent->rightNode != NULL){
                            if (root->parent->rightNode->color == RED){ //2
                                root->color = BLACK;
                                root->parent->rightNode->color = BLACK;
                                root->parent->color = RED;
                                return root;
                            }
                        }
                        RotateRight(root);
                        return root;
                    }
                    if (root->parent->rightNode == root) {
                        if (root->parent->leftNode != NULL){
                            if (root->parent->leftNode->color == RED){
                                root->color = BLACK;
                                root->parent->leftNode->color = BLACK;
                                root->parent->color = RED;
                                return root;
                            }
                        }
                        RotateRight(root);
                        RotateLeft(root->parent);
                        return root;
                    }
                }
            }
        }
    } else if (root->data.ID < data.ID){    // go tu right
        if (root->rightNode != NULL)
            RedBlackTreeInsert(root->rightNode, data);
        else
            root->rightNode = InitNode(data, RED, root);

        if (root->rightNode != NULL){
            if (root->rightNode->color == RED && root->color == RED){
                if (root->parent != NULL){
                    if (root->parent->leftNode == root){
                        if (root->parent->rightNode != NULL){
                            if (root->parent->rightNode->color == RED){ //2
                                root->color = BLACK;
                                root->parent->rightNode->color = BLACK;
                                root->parent->color = RED;
                                return root;
                            }
                        }
                        RotateLeft(root);
                        RotateRight(root->parent);
                        return root;
                    } if (root->parent->rightNode == root) {
                        if (root->parent->leftNode != NULL){
                            if (root->parent->leftNode->color == RED){
                                root->color = BLACK;
                                root->parent->leftNode->color = BLACK;
                                root->parent->color = RED;
                                return root;
                            }
                        }
                        RotateLeft(root);
                        return root;
                    }
                }
            }
        }
    } else {
        printf("INSERTED ALREADY EXISTING VALUE!!!");
    }
    return root;
}

int main(){
    Node* root = NULL;

    for (int i = 0; i < 50; i++){
        Data data;
        data.ID = i;
        sprintf(data.name, "%d", i);
        sprintf(data.surname, "%d", i);

        root = RedBlackTreeInsert(root, data);
    }

    return 0;
}
