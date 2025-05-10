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

BDD_node* initNodes(BDD_node* node, TermManager tm, HashTable** hashTable){
    if (node == NULL)
        node = getNode(NULL);

    if (tm.terms[0][0] == '1' || tm.terms[0][0] == '0'){
        node->value = tm.terms[0][0];
        return node;
    }

    deleteUnusedOrderVariables(tm);     // cleaning order string from unused variables
    node->var = tm.order[0];
    TermManager tmLow = extractSubFunction(tm, LOW);
    TermManager tmHight = extractSubFunction(tm, HIGHT);
    printf("%s\n", getFunc(tm));//
    char* funcLow = getFunc(tmLow);
    char* funcHight = getFunc(tmHight);

    ////// precess I reduction type //////
    ////// for low //////
    Data* dataLow = SearchHashTable(*hashTable, funcLow);
    if (dataLow == NULL){
        dataLow = calloc(1, sizeof(Data));
        dataLow->func = funcLow;
        node->low = initNodes(getNode(node), tmLow, hashTable);
        dataLow->link = node->low;
        *hashTable = HashTableInsert(*hashTable, dataLow);
    } else {
        node->low = dataLow->link; // reuse existing func
        free(funcLow);
    }
    ////// for hight //////
    Data* dataHight = SearchHashTable(*hashTable, funcHight);
    if (dataHight == NULL){
        dataHight = calloc(1, sizeof(Data));
        dataHight->func = funcHight;
        node->high = initNodes(getNode(node), tmHight, hashTable);
        dataHight->link = node->high;
        *hashTable = HashTableInsert(*hashTable, dataHight);
    } else {
        node->high = dataHight->link; // reuse existing func
        free(funcHight);
    }

    freeTermManager(tmLow);
    freeTermManager(tmHight);

    ////// precess S reduction type //////
    if (node->high == node->low){
        node->high->prev = node->prev;
        BDD_node* tmp = node->high;
        free(node);
        return tmp;
    }

    return node;
}

BDD *BDD_create (char* func, char* order){
    TermManager tm = getAndPrepairTerms(func, order);
    HashTable* hashTable = getHashTable(50, 0.75f);

    BDD* bdd = calloc(1, sizeof(BDD));
    bdd->root = initNodes(NULL, tm, &hashTable);
    bdd->size = hashTable->elementCount + 1; // +1 because add root

    freeTermManager(tm);

    FreeHashTable(hashTable);
    return bdd;
}

int main() {
    char func[] = "BAC+C!A+A!BC+!C!B+B!A+A!B+ABC+CBA";
    BDD * bdd = BDD_create(func, "ABC");


    return 0;
}
