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


int main(){


    return 0;
}
