#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashTable.c"
#include "termManager.c"
/*
*   Func forman = AB!C + !ABC
*   '+' = OR
*   ''  = AND
*   '!' = NOT
*/

#define HIGHT true
#define LOW false

// Main BDD structure
typedef struct BDD {
    BDD_node* root;                // pointer to the root node
    int num_vars;                  // number of variables
    int size;                      // number of nodes in the BDD
} BDD;

void BDD_free(BDD *bdd){
    // TODO: implement free of nodes
    free(bdd);
}

BDD_node* getNode(BDD_node* prev){
    BDD_node *node = calloc(1, sizeof(BDD_node));

    node->prev = prev;
    return node;
}

BDD_node* initNodes(BDD_node* node, TermManager tm, HashTable* hashTable){
    if (node == NULL)
        node = getNode(NULL);

    deleteUnusedOrderVariables(tm);     // cleaning order string from unused variables
    node->var = tm.order[0];
    TermManager tmLow = extractSubFunction(tm, LOW);
    TermManager tmHight = extractSubFunction(tm, HIGHT);
    printf("%s\n", getFunc(tm));
    printf("%s\n", getFunc(tmLow));
    printf("%s\n", getFunc(tmHight));

    return node;
}

int calculateBDDSize(BDD_node* root){
    if (root == NULL)
        return 0;
    return 1 + calculateBDDSize(root->high) + calculateBDDSize(root->low);
}

BDD *BDD_create (char* func, char* order){
    TermManager tm = getAndPrepairTerms(func, order);
    HashTable* hashTable = getHashTable(50, 0.75f);

    BDD* bdd = calloc(1, sizeof(BDD));
    bdd->root = initNodes(NULL, tm, hashTable);
    bdd->size = calculateBDDSize(bdd->root);

    freeTermManager(tm);
    FreeHashTable(hashTable);
    return bdd;
}

int main() {
    char func[] = "BAC+C!A+A!BC+!C!B+B!A+A!B+ABC+CBA";
    BDD * bdd = BDD_create(func, "ABC");


    return 0;
}
