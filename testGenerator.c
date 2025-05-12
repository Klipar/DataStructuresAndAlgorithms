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

BooleanFunctionTester getTestFunc(int maxSize){
    BooleanFunctionTester bf = {};
    bf.baseOrder = calloc(maxSize+1, sizeof(char));
    for (int i = 0; i < maxSize; i++)
        bf.baseOrder[i]=i+'A';

    TermManager tm = {};
    tm.countOfTerms = (rand() % maxSize)+1;
    tm.terms = calloc(tm.countOfTerms, sizeof(char*));
    for (int i = 0; i < tm.countOfTerms; i++){
        tm.terms[i] = calloc(maxSize*2+1, sizeof(char));
        for (int j = 0; j < maxSize; j++){
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

    char usedVariables[27] = {};
    for (int i = 0; i < strlen(bf.func); i++)
        if (strchr(usedVariables, bf.func[i]) == NULL && bf.func[i] != '!' && bf.func[i] != '+')
            *strchr(usedVariables, '\0') = bf.func[i];
    bf.countOfVariables = strlen(usedVariables);
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
            freeTermManager(tm);
            return '1';
        }
    }
    freeTermManager(tm);
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

void freeBooleanFunctionTester(BooleanFunctionTester bf){
    if (bf.func != NULL)
        free(bf.func);
    if (bf.baseOrder != NULL)
        free(bf.baseOrder);
    for (unsigned int i = 0; i < (1u << bf.countOfVariables); i++){
        free(bf.tests[i].input);
    }
    if (bf.tests != NULL)
        free(bf.tests);
}

BooleanFunctionTester getTestsData(int size){
    BooleanFunctionTester bf = getTestFunc(size);
    while (bf.countOfVariables != size || bf.func[0] == '\0' || bf.func[strlen(bf.func)-1] == '+') {
        free(bf.func);
        free(bf.baseOrder);
        bf = getTestFunc(size);    // regenerate func if count of variables is wrong
    }
    bf = generateTests(bf);
    return bf;
}
