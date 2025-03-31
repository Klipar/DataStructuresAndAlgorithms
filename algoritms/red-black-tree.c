#include <stdlib.h>
#include <stdio.h>

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

Node* deleteCase1(Node* brother);
void  deleteCase2(Node* brother);
void  deleteCase3(Node* brother);
void  deleteCase4(Node* brother);
void  deleteCase5(Node* brother);
void  deleteCase6(Node* brother);

Node* InitNode(Data data,  enum Color color, Node* parent){
    Node* root = calloc(1, sizeof(Node));
    root->color = RED;
    root->data = data;
    root->color = color;
    root->parent = parent;
    return root;
}

void RotateLeft(Node *root){
    Node *current = root->rightNode;

    current->parent = root->parent;
    if (root->parent != NULL) {
        if (root->parent->leftNode==root)
            root->parent->leftNode = current;
        else
            root->parent->rightNode = current;
    }

    root->rightNode = current->leftNode;
    if (current->leftNode != NULL)
        current->leftNode->parent = root;

    root->parent = current;
    current->leftNode = root;
}

void RotateRight(Node *root){
    Node *current = root->leftNode;

    current->parent = root->parent;
    if (root->parent != NULL) {
        if (root->parent->leftNode==root)
            root->parent->leftNode = current;
        else
            root->parent->rightNode = current;
    }

    root->leftNode = current->rightNode;
    if (current->rightNode != NULL)
        current->rightNode->parent = root;

    root->parent = current;
    current->rightNode = root;
}

void RotateLeftWithRepainting(Node* root){
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
        if (root->leftNode != NULL)
            root->leftNode->parent = tmp;
        root->leftNode = tmp;
        tmp->parent = root;
        root->color = BLACK;
        root->leftNode->color = RED;
    } else {
        printf("Critical ERROR while Left rotation: root->parent == NULL\n");
    }
}

void RotateRightWithRepainting(Node* root){
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
        if (root->leftNode != NULL)
            root->rightNode->parent = tmp;
        root->rightNode = tmp;
        tmp->parent = root;
        root->color = BLACK;
        root->rightNode->color = RED;
    } else {
        printf("Critical ERROR while Right rotation: root->parent == NULL\n");
    }
}

Node* RedBlackTreeBalance(Node* root) {
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
                        RotateLeftWithRepainting(root);
                        RotateRightWithRepainting(root);
                        continue;
                    }
                    if (root->parent->leftNode == root){
                        RotateRightWithRepainting(root->parent);
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
                        RotateRightWithRepainting(root);
                        RotateLeftWithRepainting(root);
                        continue;
                    }
                    if (root->parent->rightNode == root){
                        RotateLeftWithRepainting(root->parent);
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

Data GetMaxData (Node* root){
    while (root->rightNode != NULL){
        root->rightNode;
    }
    return root->data;
}

Node* GetBrother (Node* node){
    if (node == NULL)
        return NULL;
    if (node->parent == NULL)
        return NULL;

    if (node->parent->leftNode == node)
        return node->parent->rightNode;

    if (node->parent->rightNode == node)
        return node->parent->leftNode;

    return NULL;
}

void CutConnectWithParent (Node* current){
    if (current->parent == NULL)
        return;
    if (current->parent->leftNode == current) // cuting link in parent
        current->parent->leftNode = NULL;
    else
        current->parent->rightNode = NULL;
}

void deleteCase6(Node* current){
    Node *brother = GetBrother(current);

    brother->color = current->parent->color;
    current->parent->color = BLACK;

    if (current == current->parent->leftNode) {
        if (brother->rightNode != NULL)
            brother->rightNode->color = BLACK;
		RotateLeft(current->parent);
	} else {
        if (brother->leftNode != NULL)
		    brother->leftNode->color = BLACK;
		RotateRight(current->parent);
	}

}

void deleteCase5(Node* current){
    Node *brother = GetBrother(current);

    if (brother->color == BLACK){
        if (current == current->parent->leftNode &&
            (((brother->rightNode == NULL)) || (brother->rightNode != NULL && brother->rightNode->color == BLACK)) &&
            ((brother->leftNode != NULL) && brother->leftNode->color == RED)){
                brother->color = RED;
                brother->leftNode->color = BLACK;
                RotateRight(brother);
        } else if ( current == current->parent->rightNode &&
                    ((brother->leftNode == NULL) || brother->leftNode != NULL && brother->leftNode->color == BLACK) &&
                    ((brother->rightNode != NULL) || brother->rightNode->color == RED)){
                brother->color = RED;
                brother->rightNode->color = BLACK;
                RotateLeft(brother);
            }
    }
    deleteCase6(current);
}

void deleteCase4(Node* current){
    Node *brother = GetBrother(current);

    if (current->parent->color == RED && brother->color == BLACK &&
        (((brother->leftNode == NULL)) || (brother->leftNode != NULL && brother->leftNode->color == BLACK)) &&
        (((brother->rightNode == NULL)) || (brother->rightNode != NULL || brother->rightNode->color == BLACK))){
        brother->color = RED;
        current->parent->color = BLACK;
    } else
        deleteCase5(current);
}

void deleteCase3(Node* current){
    Node *brother = GetBrother(current);

    if ((current->parent->color == BLACK && brother->color == BLACK) &&
    (((brother->leftNode == NULL)) || (brother->leftNode != NULL && brother->leftNode->color == BLACK)) &&
    (((brother->rightNode == NULL)) || (brother->rightNode != NULL && brother->rightNode->color == BLACK))){
        brother->color = RED;
        deleteCase1(current->parent);
    } else
        deleteCase4(current);
}

void deleteCase2(Node* current){
    Node *brother = GetBrother(current);

    if (brother->color == RED){
        current->parent->color = RED;
        brother->color == BLACK;
        if (current == current->parent->leftNode)
            RotateLeft(current->parent);
        else
            RotateRight(current->parent);
    }

    deleteCase3(current);
}

Node* deleteCase1(Node* current){
    if (current->parent != NULL){
        deleteCase2(current);
        return current;
    } else
        return NULL;
}

Node* RedBlackTreeDelete (Node* root, unsigned int ID){
    Node* current = root;

    while (current != NULL){ // the current implementation does not signal an error when deleting a non-existent element!
        if (current->data.ID == ID){    // node swap < - >
            if (current->leftNode != NULL && current->rightNode != NULL){
                Data data = GetMaxData(current->leftNode);
                root = RedBlackTreeDelete(root, data.ID);
                current->data = data;
                return root;
            }

            if (current->color == RED){ // deleting leaf is red
                CutConnectWithParent(current);
                free(current);
                return root;
            } else {
                Node* son = (current->leftNode == NULL) ? current->rightNode : current->leftNode; // get son or null
                if (son != NULL){
                    if (son->color == RED){
                        current->data = son->data;
                        CutConnectWithParent(son);
                        free(son);
                        return root;
                    }
                } else {
                    root = deleteCase1(current);
                    if (root)
                        while (root->parent != NULL)
                            root = root->parent;

                    CutConnectWithParent(current);
                    free(current);
                    break;
                }
            }

        }
        if (current->data.ID > ID){
            current = current->leftNode;
            continue;
        }
        if (current->data.ID < ID){
            current = current->rightNode;
            continue;
        }
    }
    if (root != NULL)
        root->color = BLACK;
    return root;
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

    for (int i = 10; i >= 0; i--){
        root = RedBlackTreeDelete(root, i);
    }

    return 0;
}
