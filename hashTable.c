#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define BASELOADFACTOR 0.75     // [0-1]
#define BASESIZE 50         // [1 to N]

// Node of the BDD
typedef struct BDD_node {
    struct BDD_node* low;          // pointer when variable == 0
    struct BDD_node* high;         // pointer when variable == 1
    int var_index;                 // index of the variable (0, 1, ..., N-1)
    char value;                    // '0', '1', or '-' if not a terminal node
} BDD_node;

typedef struct Data {
    struct Data* next;
    BDD_node *link;
    char *func;
} Data;

typedef struct HashTable {
    Data** arr;
    unsigned size;
    unsigned elementCount;
    float loadFactor;
} HashTable;

unsigned long getStrungHash(unsigned char *string){
    unsigned long stringHash = 5381;
    int tmp;

    while (tmp = *string++)
        stringHash = ((stringHash << 5) + stringHash) + tmp; /* stringHash * 33 + tmp */

    return stringHash;
}

bool AddElementToLinkedList(Data* root, Data* node){
    if (strcmp(node->func, root->func) != 0){
        if (root->next != NULL){
            return AddElementToLinkedList(root->next, node);
        } else {
            root->next = node;
            return true;
        }
    }
    return false;
}

void FreeLinkedList(Data* root){
    if (root == NULL)
        return;

    FreeLinkedList(root->next);
    free(root->func);
    free(root);
}

void FreeHashTable(HashTable* hashTable){
    if (hashTable == NULL){
        return;
    } else{
        for (int i = 0; i < hashTable->size; i++)
            FreeLinkedList(hashTable->arr[i]);
        free (hashTable);
    }
}

Data* SearchLinkedList (Data* root, char* string){
    if (root == NULL)
        return NULL;

    if (strcmp(root->func, string))
        return SearchLinkedList(root->next, string);
    else
        return root;
}

Data* SearchHashTable (HashTable* hashTable, char* string){
    if (hashTable == NULL)
        return NULL;

    return SearchLinkedList(hashTable->arr[getStrungHash(string) % hashTable->size], string);
}

void PrintLinkedList(Data* data){
    if (data == NULL){
        printf("\n");
        return;
    }else {
        printf("%s -> ", data->func);
        PrintLinkedList(data->next);
    }
}

void PrintHasTable (HashTable* hashTable){
    if (hashTable == NULL){
        printf("======================================\n");
        printf("Map is NULL.");
    } else{
        printf("======================================\n");
        printf("Size = %d\n", hashTable->size);
        printf("Element count = %d\n", hashTable->elementCount);
        for (int i = 0; i < hashTable->size; i++){
            printf("[%d]| ", i);
            PrintLinkedList(hashTable->arr[i]);
        }
    }
}

HashTable* HashTableInsert (HashTable* hashTable, Data* data){
    if (hashTable == NULL)
        hashTable = getHashTable(BASESIZE, BASELOADFACTOR);

    if ((float)hashTable->elementCount/(float)hashTable->size >= hashTable->loadFactor){
        HashTable* newTable = getHashTable(hashTable->size*2, hashTable->loadFactor);

        for (int i = 0; i < hashTable->size; i++){ //copy all previous data
            Data* element = hashTable->arr[i];
            while (element != NULL){
                newTable = HashTableInsert(newTable, element);
                element = element->next;
            }
        }

        free(hashTable);        // free only hashTable because all data still important
        hashTable = newTable;
    }

    unsigned int arrId = getStrungHash(data->func) % hashTable->size;
    if (hashTable->arr[arrId] == NULL){
        hashTable->arr[arrId] = data;
        hashTable->elementCount++;
    } else {
        if(AddElementToLinkedList(hashTable->arr[arrId], data)){
            hashTable->elementCount++;
        } else{
            if (SearchLinkedList(hashTable->arr[arrId], data->func) != data){
                free(data->func)
                free(data);    // free extra copied data
            }
        }
    }
    return hashTable;
}

HashTable* getHashTable(unsigned size, float loadFactor){
    HashTable* hashTable = calloc (1, sizeof(HashTable));
    hashTable->size = size;
    hashTable->arr = calloc(size, sizeof(Data*));
    hashTable->loadFactor = loadFactor;
    return hashTable;
}

bool HashTableDelate(HashTable* hashTable, char *string){
    if (hashTable == NULL)
        return false;

    unsigned int id = getStrungHash(string) % hashTable->size;
    if (hashTable->arr[id] != NULL){
        if (strcmp(hashTable->arr[id]->func, string) == 0){
            Data* dump = hashTable->arr[id];
            hashTable->arr[id] = hashTable->arr[id]->next;
            free(dump->func);
            free(dump);
            hashTable->elementCount--;
            return true;
        }

        Data* previous = hashTable->arr[id];
        Data* current = hashTable->arr[id]->next;
        while (current != NULL){
            if (strcmp(hashTable->arr[id]->func, string) == 0){
                previous->next = current->next;
                free(current->func);
                free(current);
                hashTable->elementCount--;
                return true;
            } else{
                previous = current;
                current = current->next;
            }
        }
        return false;
    }
}

int main(){
    // HashTable* hashTable;

    // for (int i = 0; i <= 50; i++){
    //     Data* data = calloc(1, sizeof(Data));
    //     data->ID = i;
    //     sprintf(data->name, "%d", i);
    //     sprintf(data->surname, "%d", i);

    //     hashTable = HashTableInsert(hashTable, data);
    // }

    // for (int i = 0; i <= 50; i++){
    //     if (SearchHashTable(hashTable, i) != NULL)
    //         printf("%d\n", SearchHashTable(hashTable, i)->ID);
    //     else{
    //         printf("NotFound = %d", i);
    //         SearchHashTable(hashTable, i);
    //     }
    // }
    // PrintHasTable(hashTable);
    // for (int i = 0; i <= 50; i++){
    //     if (!HashTableDelate(hashTable, i)){
    //         printf("ERROR");
    //     }
    // }
    // PrintHasTable(hashTable);

    return 0;
}
