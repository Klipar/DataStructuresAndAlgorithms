#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "termManager.c"
typedef struct Test {
    char* input; // in ABC order
    char output; // {'1', '0', -1} // -1 if wrong input
} Test;

typedef struct BooleanFunctionTester {
    char* func;
    char* baseOrder;
    int countOfVariables;
    int countOfTerms;
    Test* tests;
} BooleanFunctionTester;

BooleanFunctionTester getTestFunc(int size){
    BooleanFunctionTester bf = {};
    bf.countOfVariables = size;
    bf.baseOrder = calloc(size+1, sizeof(char));
    for (int i = 0; i < size; i++)
        bf.baseOrder[i]=i+'A';

    srand(time(NULL));

    TermManager tm = {};
    tm.countOfTerms = (rand() % size)+1;
    tm.terms = calloc(tm.countOfTerms, sizeof(char*));
    for (int i = 0; i < tm.countOfTerms; i++){
        tm.terms[i] = calloc(size*2+1, sizeof(char));
        for (int j = 0; j < size; j++){
            if ((rand() % 100)+1 > 50){
                char addVariable[3] = {0};
                if ((rand() % 100)+1 > 50){
                    addVariable[0] = '!';
                    addVariable[1] = bf.baseOrder[j];
                    addVariable[2] = '\0';
                    strcat(tm.terms[i], addVariable);
                } else {
                    addVariable[0] = bf.baseOrder[j];
                    addVariable[1] = '\0';
                    strcat(tm.terms[i], addVariable);
                }
            }
        }
    }
    bf.countOfTerms = tm.countOfTerms;
    bf.func = getFunc(tm);
    freeTermManager(tm);
    for (int i = 0; i < strlen(bf.func)-1; i++){
        if (bf.func[i] == '+' && bf.func[i+1] == '+'){
            strcpy(bf.func+i, bf.func+i+1);
            i = -1;
            bf.countOfTerms--;
        }
    }
    return bf;
}

char* intToString(unsigned int inp, int countOfVariables){
    char* result = calloc(countOfVariables+1, sizeof(char));
    unsigned int mask = 1;
    for (int i = countOfVariables-1; i >=0; i--){
        if ((inp & mask) > 0){
            result[i] = '1';
        } else {
            result[i] = '0';
        }
        mask = mask << 1;
    }
    return result;
}

char getFuncResult(char* func, char* order, char* inp){
    TermManager tm = getAndPrepairTerms(func, order);
    for (int i = 0; i < tm.countOfTerms; i++){
        for (int j = 0; j < strlen(tm.terms[i]); j++){
            bool isNeg = false;
            if (tm.terms[i][j] == '!'){
                j++;
                isNeg = true;
            }
            if((inp[(strchr(order, tm.terms[i][j]) - order)] == '0' &&
                isNeg == false) ||
                (inp[(strchr(order, tm.terms[i][j]) - order)] == '1' &&
                isNeg == true)) {

                tm.terms[i][0] = '\0';
                break;
            }
        }
    }

    for (int i = 0; i < tm.countOfTerms; i++){
        if (tm.terms[i][0] != '\0'){
            return '1';
        }
    }
    return '0';
}

BooleanFunctionTester generateTests(BooleanFunctionTester bf){
    bf.tests = calloc((1u << bf.countOfVariables), sizeof(Test));
    for (unsigned int i = 0; i < (1u << bf.countOfVariables); i++){
        Test test;
        test.input = intToString(i, bf.countOfVariables);
        test.output = getFuncResult(bf.func, bf.baseOrder, test.input);
        bf.tests[i] = test;
    }
    return bf;
}

BooleanFunctionTester getTestsData(int size){
    BooleanFunctionTester bf = getTestFunc(size);
    bf = generateTests(bf);
    return bf;
}

void freeBooleanFunctionTester(BooleanFunctionTester bf){
    if (bf.func != NULL)
        free(bf.func);
    if (bf.baseOrder != NULL)
        free(bf.baseOrder);
    if (bf.tests != NULL)
        free(bf.tests);
}
