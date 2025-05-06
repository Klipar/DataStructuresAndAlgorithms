#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int num_vars;                  // number of variables
    int size;                      // number of nodes in the BDD
} BDD;

// Term manager for working with terms
typedef struct TermManager {
    char **terms;
    int countOfTerms;
} TermManager;

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

void sortInTerms(char* func, char* order){
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

void sortTermsByWeight(TermManager tm, char* order){
    for (int i = 0; i < tm.countOfTerms-1; i++){
        if (getTermWeight(tm.terms[i], order) < getTermWeight(tm.terms[i+1], order)){
            swapTerms(tm, i);
            i = -1;
            continue;
        }
    }
}

void sortTerms(TermManager tm, char* order){
    sortTermsByWeight(tm, order);
    sortTermsBySize(tm);
}

BDD *BDD_create (char* func, char* order){
    BDD* bdd = calloc(1, sizeof(BDD));

    sortInTerms(func, order);

    TermManager tm = getTerms(func);
    sortTerms(tm, order);

    return bdd;
}

int main() {
    char func[] = "BAC+C!A+A!BC+!C!B+B!A+A!B+ABC+CBA";
    BDD * bdd = BDD_create(func, "ABC");


    return 0;
}
