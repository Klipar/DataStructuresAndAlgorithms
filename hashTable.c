#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define BASELOADFACTOR 0.75     // [0-1]
#define BASESIZE 50         // [1 to N]

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

#ifndef DATA_H
#define DATA_H
typedef struct Data {
    struct Data* next;
    BDD_node* link;
    char* func;
} Data;
#endif // DATA_H

#ifndef HASH_TABLE_H
#define HASH_TABLE_H
typedef struct HashTable {
    Data** arr;
    unsigned size;
    unsigned elementCount;
    float loadFactor;
} HashTable;
#endif // HASH_TABLE_H

HashTable* getHashTable(unsigned size, float loadFactor){
    HashTable* hashTable = calloc (1, sizeof(HashTable));
    hashTable->size = size;
    hashTable->arr = calloc(size, sizeof(Data*));
    hashTable->loadFactor = loadFactor;
    return hashTable;
}

unsigned long getStringHash(unsigned char *string){
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

    return SearchLinkedList(hashTable->arr[getStringHash(string) % hashTable->size], string);
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
        hashTable = getHashTable(BASESIZE, (float)BASELOADFACTOR);

    if ((float)hashTable->elementCount/(float)hashTable->size >= hashTable->loadFactor){
        HashTable* newTable = getHashTable(hashTable->size*2, hashTable->loadFactor);

        for (int i = 0; i < hashTable->size; i++){ //copy all previous data
            Data* element = hashTable->arr[i];
            while (element != NULL){
                Data* next = element->next;
                element->next = NULL;
                newTable = HashTableInsert(newTable, element);
                element = next;
            }
        }

        free(hashTable);        // free only hashTable because all data still important
        hashTable = newTable;
    }

    unsigned int arrId = getStringHash(data->func) % hashTable->size;
    if (hashTable->arr[arrId] == NULL){
        hashTable->arr[arrId] = data;
        hashTable->elementCount++;
    } else {
        if(AddElementToLinkedList(hashTable->arr[arrId], data)){
            hashTable->elementCount++;
        } else{
            if (SearchLinkedList(hashTable->arr[arrId], data->func) != data){
                free(data->func);
                free(data);    // free extra copied data
            }
        }
    }
    return hashTable;
}

bool HashTableDelate(HashTable* hashTable, char *string){
    if (hashTable == NULL)
        return false;

    unsigned int id = getStringHash(string) % hashTable->size;
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
            if (strcmp(current->func, string) == 0){
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

// int main() {
//     FILE *file = fopen("tests/tests/test_10000000.txt", "r");
//     if (!file) {
//         perror("Failed to open file!");
//         return 1;
//     }

//     HashTable* hashTable = getHashTable(50, 0.75f);

//     char buffer[1024];
//     while (fgets(buffer, sizeof(buffer), file)) {
//         if (buffer[0] == '\n' || buffer[0] == '\0') continue;
//         buffer[strcspn(buffer, "\n")] = '\0';

//         if (buffer[0] == 'i') {
//             buffer[0] = '_';
//             Data* data = calloc(1, sizeof(Data));
//             data->func = calloc(strlen(buffer) + 1, sizeof(char));
//             strcpy(data->func, buffer);
//             hashTable = HashTableInsert(hashTable, data);
//         } else if (buffer[0] == 's') {
//             buffer[0] = '_';
//             if (SearchHashTable(hashTable, buffer) == NULL){
//                 printf("fail to fond string -> \n%s", buffer);
//             }
//         } else if (buffer[0] == 'r') {
//             buffer[0] = '_';
//             if (!HashTableDelate(hashTable, buffer)){
//                 printf("fail to delete string -> \n%s", buffer);
//             }
//         }
//     }

//     free(hashTable);
//     fclose(file);
//     return 0;
// }
