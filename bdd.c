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

#ifndef BDD_NODE_H
#define BDD_NODE_H
// Node of the BDD
typedef struct BDD_node {
    struct BDD_node* low;          // pointer when variable == 0
    struct BDD_node* high;         // pointer when variable == 1
    struct BDD_node* prev;         // pointer to previous node == 1
    char var;                      // variable name {A, B, C, ...}
    char value;                    // '0', '1', or '-' if not a terminal node
} BDD_node;
#endif // BDD_NODE_H

// Main BDD structure
typedef struct BDD {
    BDD_node* root;                // pointer to the root node
    int num_vars;                  // number of variables
    int size;                      // number of nodes in the BDD
} BDD;

#ifndef TermManagerH
#define TermManagerH
// Term manager for working with terms
typedef struct TermManager {
    char **terms;
    int countOfTerms;
    char* order;
} TermManager;
#endif

void BDD_free(BDD *bdd){
    // TODO: implement free of nodes
    free(bdd);
}

void swapChars(char* string, int i, int j){
    char tmp = string[i];
    string[i] = string[j];
    string[j] = tmp;
}

void swapVariable(char* string, int i){ // index of first variable. swap only variables witch is nearby ich other
    if (string == NULL)
        return;

    if (strlen(string) < i+1){
        perror("Error while swapping variables!\n");
        return;
    }

    if (i > 0 && string[i-1] == '!' && string[i+1] == '!'){     // !A!B
        swapChars(string, i, i+2);      // !B!A
        return;
    }

    if ((i > 0 && string[i-1] != '!' && string[i+1] != '!') ||  // AB
        (i == 0 && string[i+1] != '!')){
        swapChars(string, i, i+1);      // BA
        return;
    }

    if (i > 0 && string[i-1] == '!' && string[i+1] != '!'){     // !AB
        swapChars(string, i, i+1);      // !BA
        swapChars(string, i-1, i);      // B!A
        return;
    }

    if (((i > 0 && string[i-1] != '!') || (i == 0)) && (string[i+1] == '!')){ // A!B
        swapChars(string, i, i+1);      // !AB
        swapChars(string, i+1, i+2);    // !BA
        return;
    }
}

void sortOneTerm(char* func, char* order){
    for (int i = 0; i < strlen(func)-1; i++){ // pointer on func
        if (func[i] == '+' || func[i+1] == '+' || func[i] == '!') // we are in the middle between terms OR at !
            continue;
        int j = i+1;
        if (func[j] == '!')
            j++;

        if (strchr(order, func[i]) > strchr(order, func[j])){
            swapVariable(func, i);
            i = -1;
            continue;
        }
    }
}

void printTerms(TermManager tm){
    for (int i = 0; i < tm.countOfTerms; i++){
        printf("%s\n", tm.terms[i]);
    }
}

TermManager getTerms (char* func){
    TermManager tm = {NULL, 1};

    for (int i = 0; i < strlen(func); i++) {
        if (func[i] == '+'){
            tm.countOfTerms++;
        }
    }

    tm.terms = calloc(tm.countOfTerms, sizeof(char*));
    int currentTerm = 0;
    int sizeOfTerm = 0;
    for (int i = 0; i < strlen(func); i++){
        for (sizeOfTerm = 0; sizeOfTerm+i < strlen(func); sizeOfTerm++){
            if (func[sizeOfTerm+i] == '+')
                break;
        }
        tm.terms[currentTerm] = malloc(sizeOfTerm*sizeof(char));
        for (int k = 0; k <= sizeOfTerm; k++){
            if (func[k+i] != '+')
            tm.terms[currentTerm][k] = func[k+i];
        }
        i += sizeOfTerm;
        currentTerm++;
    }

    return tm;
}

int getTermSize(char *term){
    int size = 0;
    for (int i = 0; i < strlen(term); i++){
        if (term[i] != '+' && term[i] != '!'){
            size++;
        }
    }
    return size;
}

void swapTerms(TermManager tm, int i){ //swap i and i+1 terms
    char *tmp = tm.terms[i];
    tm.terms[i] = tm.terms[i+1];
    tm.terms[i+1] = tmp;
}

void sortTermsBySize(TermManager tm){
    for (int i = 0; i < tm.countOfTerms-1; i++){
        if(getTermSize(tm.terms[i]) > getTermSize(tm.terms[i+1])){
            swapTerms(tm, i);
            i = -1;
            continue;
        }
    }
}

unsigned long long calculateWeightIteration(unsigned long long variable, int addValue){
    variable *= 10;
    variable += addValue;
    return variable;
}

unsigned long long getTermWeight(char* term, char* order){
    /* // system of calculating weight of term //
    * ''   = 1
    * '!A' = 2
    * 'A'  = 3
    *
    * order == "ABC"
    *  ABC == 333
    * !ABC == 233
    *    C == 113
    * !A!C == 212
    *     ...
    */

    unsigned long long weight = 0;
    int tPointer = 0;       // term pointer
    int oPointer = 0;       // order pointer

    while (oPointer < strlen(order)){
        if (tPointer < strlen(term)){
            if (term[tPointer] == '!'){
                tPointer++;
            }

            if (term[tPointer] != order[oPointer]){
                weight = calculateWeightIteration(weight, 1);
                oPointer++;
                continue;
            }

            if (term[tPointer] == order[oPointer]) {
                if (tPointer > 0 && term[tPointer-1] == '!'){
                    weight = calculateWeightIteration(weight, 2);
                    tPointer++;
                    oPointer++;
                } else {
                    weight = calculateWeightIteration(weight, 3);
                    tPointer++;
                    oPointer++;
                }
            }
        } else {
            weight = calculateWeightIteration(weight, 1);
            oPointer++;
        }
    }
    return weight;
}

void sortTermsByWeight(TermManager tm){
    for (int i = 0; i < tm.countOfTerms-1; i++){
        if (getTermWeight(tm.terms[i], tm.order) < getTermWeight(tm.terms[i+1], tm.order)){
            swapTerms(tm, i);
            i = -1;
            continue;
        }
    }
}

void sortInTerms(TermManager tm){
    for (int i = 0; i < tm.countOfTerms; i++){
        sortOneTerm(tm.terms[i], tm.order);
    }
}

void sortTerms(TermManager tm){
    sortInTerms(tm);
    sortTermsByWeight(tm);
    sortTermsBySize(tm);
}

void freeTermManager(TermManager tm){
    free(tm.terms);
}

char* getFunc(TermManager tm){ // important to free after using this func
    if (tm.terms == NULL)
        return NULL;
    int stringSize = 1;
    for (int i = 0; i < tm.countOfTerms; i++){
        stringSize+= strlen(tm.terms[i]);
    }

    stringSize += tm.countOfTerms-1; // add space for'+'

    char* str = calloc(stringSize, sizeof(char));

    for (int i = 0; i < tm.countOfTerms; i++){
        if (i != 0){
            str[strlen(str)+1] = '\0';
            str[strlen(str)] = '+';
        }
        strcat(str, tm.terms[i]);
    }
    return str;
}

TermManager copyTermManager(TermManager tm){  // get full copy
    char **terms;
    terms = calloc(tm.countOfTerms, sizeof(char *));
    for (int i = 0; i < tm.countOfTerms; i++){
        terms[i] = calloc(strlen(tm.terms[i])+1, sizeof(char));
        strcpy(terms[i], tm.terms[i]);
    }
    tm.terms = terms;
    return tm;
}

TermManager getSubTerms (TermManager tm, char var){ // change the original tm
    for (int i = 0; i < tm.countOfTerms; i++){
        if (tm.terms[i][0] == var){
            strcpy(tm.terms[i], tm.terms[i] + 1);
        } else if (tm.terms[i][0] == '!'){
            strcpy(tm.terms[i], tm.terms[i] + 2);
        }
        if(tm.terms[i][0] == '\0'){     //delete the cleaned node;
            free(tm.terms[i]);
            for (int j = i; j < tm.countOfTerms-1; j++){
                tm.terms[j] = tm.terms[j+1];
            }
            tm.countOfTerms--;
        }
    }
    return tm;
}

int calculateSize(BDD_node* root){
    if (root == NULL)
        return 0;
    return 1 + calculateSize(root->high) + calculateSize(root->low);
}

TermManager getAndPrepairTerms(char* func, char* order){
    TermManager tm = getTerms(func);
    tm.order = calloc(strlen(order)+1, sizeof(char));
    strcpy(tm.order, order);
    sortTerms(tm);
    return tm;
}

BDD_node* initNodes(BDD_node* node, TermManager tm, HashTable* hashTable){
    return NULL;
}

BDD *BDD_create (char* func, char* order){
    BDD* bdd = calloc(1, sizeof(BDD));

    TermManager tm = getAndPrepairTerms(func, order);
    HashTable* hashTable = getHashTable(50, 0.75f);

    bdd->root = initNodes(NULL, tm, hashTable);
    bdd->size = calculateSize(bdd->root);

    freeTermManager(tm);
    FreeHashTable(hashTable);
    return bdd;
}

int main() {
    char func[] = "BAC+C!A+A!BC+!C!B+B!A+A!B+ABC+CBA";
    BDD * bdd = BDD_create(func, "ABC");


    return 0;
}
