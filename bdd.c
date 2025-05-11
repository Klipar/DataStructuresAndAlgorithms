#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashTable.c"
/*
*   Func forman = AB!C + !ABC
*   '+' = OR
*   ''  = AND
*   '!' = NOT
*/
#include "testGenerator.c"

#define HIGHT true
#define LOW false

// Main BDD structure
typedef struct BDD {
    BDD_node* root;                // pointer to the root node
    int num_vars;                  // number of variables
    int size;                      // number of nodes in the BDD
    HashTable* hashTable;
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

BDD_node* initNodes(BDD_node* prev, TermManager tm, HashTable** hashTable){
    BDD_node* node = getNode(prev);

    if (tm.terms[0][0] == '1' || tm.terms[0][0] == '0'){
        node->value = tm.terms[0][0];
        return node;
    }

    deleteUnusedOrderVariables(tm);     // cleaning order string from unused variables
    node->var = tm.order[0];
    TermManager tmLow = extractSubFunction(tm, LOW);
    TermManager tmHight = extractSubFunction(tm, HIGHT);
    // printf("%s\n", getFunc(tm));//
    char* funcLow = getFunc(tmLow);
    char* funcHight = getFunc(tmHight);

    TermManager tm1 = getAndPrepairTerms(funcLow, tmLow.order);
    TermManager tm2 = getAndPrepairTerms(funcHight, tmHight.order);
    char* tm1FuncLow = getFunc(tm1);
    char* tm2FuncHight = getFunc(tm2);
    freeTermManager(tm1);
    freeTermManager(tm2);

    ////// precess S reduction type //////
    if (strcmp(tm1FuncLow, tm2FuncHight) == 0){
        // node->high->prev = node->prev;
        BDD_node* tmp = initNodes(node->prev, tmLow, hashTable);
        free(node);
        freeTermManager(tmLow);
        freeTermManager(tmHight);
        free(funcLow);
        free(funcHight);
        free(tm1FuncLow);
        free(tm2FuncHight);
        return tmp;
    }
    free(tm1FuncLow);
    free(tm2FuncHight);

    ////// precess I reduction type //////
    ////// for low //////
    Data* dataLow = SearchHashTable(*hashTable, funcLow);
    if (dataLow == NULL){
        dataLow = calloc(1, sizeof(Data));
        dataLow->func = funcLow;
        node->low = initNodes(node, tmLow, hashTable);
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
        node->high = initNodes(node, tmHight, hashTable);
        dataHight->link = node->high;
        *hashTable = HashTableInsert(*hashTable, dataHight);
    } else {
        node->high = dataHight->link; // reuse existing func
        free(funcHight);
    }

    freeTermManager(tmLow);
    freeTermManager(tmHight);

    return node;
}

BDD *BDD_create (char* func, char* order){
    TermManager tm = getAndPrepairTerms(func, order);

    BDD* bdd = calloc(1, sizeof(BDD));
    bdd->hashTable = getHashTable(50, 0.75f);
    bdd->root = initNodes(NULL, tm, &bdd->hashTable);
    bdd->size = bdd->hashTable->elementCount + 1; // +1 because add root

    freeTermManager(tm);

    return bdd;
}

void freeBDD(BDD *bdd){
    free(bdd->root);
    FreeHashTable(bdd->hashTable);
    free(bdd);
}

void swap(char *x, char *y) {
    char temp = *x;
    *x = *y;
    *y = temp;
}

char** getOrders(char *order) {
    int n = strlen(order);
    int *index = calloc(n, sizeof(int));
    char** result = calloc(strlen(order), sizeof(char*));

    for(int i = 0, j = 0; i < n && j < strlen(order); i++){
        if (index[i] < i) {
            swap(&order[i % 2 == 0 ? 0 : index[i]], &order[i]);
            result[j] = calloc(strlen(order)+1, sizeof(char));
            strcpy(result[j], order);
            index[i]++;
            i = -1;
            j++;
        } else
            index[i] = 0;
    }

    free(index);

    return result;
}

BDD *BDD_create_with_best_order (char* func){
    char baseOrder[27] = {};
    for (int i = 0; i < strlen(func); i++)
        if (strchr(baseOrder, func[i]) == NULL && func[i] != '!' && func[i] != '+')
            *strchr(baseOrder, '\0') = func[i];

    BDD *bddOutput = BDD_create(func, baseOrder);
    printf("%i\n", bddOutput->size);
    char** orders = getOrders(baseOrder);

    for (int i = 0; i < strlen(baseOrder); i++){
        BDD *newOutput = BDD_create(func, orders[i]);
        if (newOutput->size < bddOutput->size){
            freeBDD(bddOutput);
            bddOutput = newOutput;
        } else {
            freeBDD(newOutput);
        }
    }

    for (int i = 0; i < strlen(baseOrder); i++)
        free(orders[i]);
    free(orders);
    printf("%i\n", bddOutput->size);
    return bddOutput;
}

char BDD_use (BDD *bdd, char* inp){
    BDD_node* current = bdd->root;
    while (current->high != NULL){
        if (strlen(inp)-1 < current->var-'A'){
            return -1; // not enough variables in input
        }
        if (inp[current->var-'A'] == '1'){
            current = current->high;
        }else{
            current = current->low;
        }
    }
    return current->value;
}

int main() {
    BooleanFunctionTester bf = getTestsData(13);

    BDD * bdd = BDD_create_with_best_order(bf.func);
    for (unsigned int i = 0; i < (1u << bf.countOfVariables); i++){
        if (BDD_use(bdd, bf.tests[i].input) != bf.tests[i].output)
            printf("FAIL ON TEST: func:\n%s\nwith input == %s\nexpected == %c\nreal == %c\n", bf.func, bf.tests[i].input, bf.tests[i].output, BDD_use(bdd, bf.tests[i].input));
        else{
            // printf("success!!!!!\n");
        }
    }
    freeBooleanFunctionTester(bf);
    freeBDD(bdd);
    return 0;
}
