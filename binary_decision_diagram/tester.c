#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bdd.c"

////// testing settings //////
#define MIN_VARIABLES 4
#define MAX_VARIABLES 20
#define COUNT_OF_ITERATIONS 100
//////////////////////////////

typedef struct TestAlgorithms {
    clock_t BDD_create_time;
    clock_t BDD_create_with_best_order_time;
    clock_t BDD_use_time;
    unsigned long int averageMemoryUsage;
    unsigned int averageCountOfNodesInRegularCreation;
    unsigned int averageCountOfNodesInRegularBestCreation;
} TestAlgorithms;

void printTestesData (TestAlgorithms test, int numVariables){
    printf("\nResults:\n");
    printf("Average creation time with regular order (ms): %.4f\n", ((double)test.BDD_create_time / CLOCKS_PER_SEC * 1000.0)/COUNT_OF_ITERATIONS);
    printf("Average creation time with best order (ms)   : %.4f\n", ((double)test.BDD_create_with_best_order_time / CLOCKS_PER_SEC * 1000.0)/COUNT_OF_ITERATIONS);
    printf("Average use time (ms)                        : %.4f\n", ((double)test.BDD_use_time / CLOCKS_PER_SEC * 1000.0)/(COUNT_OF_ITERATIONS*(1u << numVariables)));
    printf("Average memory usage (Kb)                    : %lu\n", (test.averageMemoryUsage/COUNT_OF_ITERATIONS)/1024);
    printf("Average count of nodes with regular creation : %u\n", test.averageCountOfNodesInRegularCreation/COUNT_OF_ITERATIONS);
    printf("Average count of nodes with best creation    : %u\n", test.averageCountOfNodesInRegularBestCreation/COUNT_OF_ITERATIONS);
}

unsigned long int getSize(BDD *bdd){
    unsigned long int size = 0;
    size += sizeof(BDD*);
    size += sizeof(BDD);
    size += sizeof(BDD_node)*bdd->size;
    size += sizeof(HashTable);
    size += sizeof(Data*)*bdd->hashTable->size;
    size += sizeof(Data)*bdd->hashTable->elementCount;

    for (int i = 0; i < bdd->hashTable->size; i++){
        Data* tmp = bdd->hashTable->arr[i];
        while(tmp != NULL){
            size += sizeof(char)*strlen(tmp->func);
            tmp = tmp->next;
        }
    }
    return size;
}

int main() {
    srand(time(NULL));
    for (int numVariables = MIN_VARIABLES; numVariables < MAX_VARIABLES; numVariables++){
        TestAlgorithms test = {};
        printf("\n=======================================================\n");
        printf("Testing bdd with %i variables\n", numVariables);
        for (int iteration = 0; iteration < COUNT_OF_ITERATIONS; iteration++){
            printf("\rPerform the test: %i/%i", iteration+1, COUNT_OF_ITERATIONS);
            char bddUseResult; // result of use func
            clock_t timestamp;

            ///// creating test data /////
            BooleanFunctionTester bf = getTestsData(numVariables);

            ///// regular bdd create //////
            timestamp = clock();
            BDD *bddRegular = BDD_create(bf.func, bf.baseOrder);
            test.BDD_create_time += clock() - timestamp;
            test.averageCountOfNodesInRegularCreation += bddRegular->size;
            freeBDD(bddRegular);


            ///// bdd create with best order /////
            timestamp = clock();
            BDD * bdd = BDD_create_with_best_order(bf.func);
            test.BDD_create_with_best_order_time += clock() - timestamp;
            test.averageCountOfNodesInRegularBestCreation += bdd->size;
            test.averageMemoryUsage += getSize(bdd);

            //// bdd check ////
            for (unsigned int i = 0; i < (1u << bf.countOfVariables); i++){
                timestamp = clock();
                bddUseResult = BDD_use(bdd, bf.tests[i].input);
                test.BDD_use_time += clock() - timestamp;

                if (bddUseResult != bf.tests[i].output)
                    printf("FAIL ON TEST: func:\n%s\nwith input == %s\nexpected == %c\nreal == %c\n", bf.func, bf.tests[i].input, bf.tests[i].output, bddUseResult);
            }

            freeBooleanFunctionTester(bf);
            freeBDD(bdd);
        }
        printTestesData(test, numVariables);
    }
    return 0;
}
