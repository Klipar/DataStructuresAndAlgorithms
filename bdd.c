#include <stdio.h>
#include <stdlib.h>

/*
*   Func forman = AB!C + !ABC
*   '+' = OR
*   ''  = AND
*   '!' = NOT
*/

// Node of the BDD
typedef struct BDD_node {
    struct BDD_node* low;          // pointer when variable == 0
    struct BDD_node* high;         // pointer when variable == 1
    int var_index;                 // index of the variable (0, 1, ..., N-1)
    char value;                    // '0', '1', or '-' if not a terminal node
} BDD_node;

// Main BDD structure
typedef struct BDD {
    BDD_node* root;                // pointer to the root node
    char* variable_order;          // order of variables (array of variable indices)
    int num_vars;                  // number of variables
    int size;                      // number of nodes in the BDD
} BDD;

void BDD_free(BDD *bdd){
    // TODO: implement free of nodes
    free(bdd);
}

BDD *BDD_create (char* func, char* order){
    BDD* bdd = calloc(1, sizeof(BDD));

    return bdd;
}

int main() {
    BDD * bdd = BDD_create("ABC+A!C", "ABC");


    return 0;
}
