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

BDD_node* initNodes(BDD_node* node, TermManager tm, HashTable* hashTable){
    return NULL;
}

int calculateBDDSize(BDD_node* root){
    if (root == NULL)
        return 0;
    return 1 + calculateBDDSize(root->high) + calculateBDDSize(root->low);
}

BDD *BDD_create (char* func, char* order){
    BDD* bdd = calloc(1, sizeof(BDD));

    TermManager tm = getAndPrepairTerms(func, order);
    HashTable* hashTable = getHashTable(50, 0.75f);

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
