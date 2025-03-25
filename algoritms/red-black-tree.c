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
    struct Node* leftNode;
    struct Node* rightNode;
    struct Node* parent;
} Node;

Node* InitNode(Data data,  enum Color color, Node* parent){
    Node* root = calloc(1, sizeof(Node));
    root->color = RED;
    root->data = data;
    root->color = color;
    root->parent = parent;
    return root;
}

Node* InitNode(Data data){      // red node
    Node* root = calloc(1, sizeof(Node));
    root->color = RED;
    root->data = data;
    return root;
}

Node* InitNode(Data data, Node* parent){      // red node with parent
    Node* root = calloc(1, sizeof(Node));
    root->color = RED;
    root->data = data;
    root->parent = parent;
    return root;
}

Node* RedBlackTreeInsert (Node* root, Data data){
    if (root == NULL)
        return InitNode(data, BLACK);


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
