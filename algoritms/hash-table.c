#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define LOADFACTOR 0.75     // [0-1]
#define DEFAULT_SIZE 10


typedef struct Data {
    unsigned int ID;
    char name [20];
    char surname [20];
} Data;

typedef struct HashTable {
    Data* arr;
    unsigned size;
    unsigned elementCount;
    float loadFactor;
} HashTable;

Data* SearchHashTable (HashTable* hashTable, int ID){
    if (hashTable == NULL)
        return NULL;

    return SearchLinkedList(hashTable->arr[ID % hashTable->size], ID);
}

void HasTablePrint (HashTable* hashTable){
    if (hashTable == NULL){
        printf("======================================\n");
        printf("Map is NULL.");
    } else{
        printf("======================================\n");
        printf("Size = %d\n", hashTable->size);
        printf("loadFactor = %f\n", hashTable->loadFactor);
        printf("Element count = %d\n", hashTable->elementCount);
        unsigned int size = hashTable->size, count = 0;
        do {
            count++;
            size /= 10;
        } while (size > 0);
        for (int i = 0; i < hashTable->size; i++){
            printf("[%0*d] ID: %u\t %s \t %s\n", count, i, (hashTable->arr+i)->ID, (hashTable->arr+i)->name, (hashTable->arr+i)->surname);
        }
    }
}

HashTable* HashTableInsert (HashTable* hashTable, Data* data){
    if (hashTable == NULL){
        hashTable = calloc (1, sizeof(HashTable));
        hashTable->size = BASESIZE;
        hashTable->arr = calloc(hashTable->size, sizeof(Data*));
    }

    if ((float)hashTable->elementCount/(float)hashTable->size >= LOADFACTOR){
        HashTable* newTable = calloc (1, sizeof(HashTable));
        newTable->size = hashTable->size*2;
        newTable->arr = calloc(newTable->size, sizeof(Data*));

        for (int i = 0; i < hashTable->size; i++) //copy all previous data
            if (hashTable->arr[i] != NULL)
                newTable = HashTableInsert(newTable, hashTable->arr[i]);

        free(hashTable);
        hashTable = newTable;
    }
    unsigned int arrId = data->ID % hashTable->size;
    if (hashTable->arr[arrId] == NULL){
        hashTable->arr[arrId] = data;
        hashTable->elementCount++;
    } else
        if(AddElementToLinkedList(hashTable->arr[arrId], data)){
            hashTable->elementCount++;
        } else{
            free (data);
        }
    return hashTable;
}

bool HashTableDelate(HashTable* hashTable, int ID){
    if (hashTable == NULL)
        return false;
    unsigned int id = ID % hashTable->size;
    if (hashTable->arr[id] != NULL){
        if (hashTable->arr[id]->ID == ID){
            Data* dump = hashTable->arr[id];
            hashTable->arr[id] = hashTable->arr[id]->next;
            free (dump);
            hashTable->elementCount--;
            return true;
        }

        Data* previous = hashTable->arr[id];
        Data* current = hashTable->arr[id]->next;
        while (current != NULL){
            if (current->ID == ID){
                previous->next = current->next;
                free (current);
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

HashTable* HashTableInit(unsigned int size, float loadFactor){
    HashTable* table = malloc (sizeof(HashTable));

    if (size <= 0 || loadFactor > 1 || loadFactor <= 0)
        return NULL;

    table->size = size;
    table->arr = calloc(table->size, sizeof(Data));
    table->elementCount = 0;
    table->loadFactor = loadFactor;
    return table;
}

int main(){
    HashTable* hashTable;

    for (int i = 0; i <= 50; i++){
        Data* data = calloc(1, sizeof(Data));
        data->ID = i;
        sprintf(data->name, "%d", i);
        sprintf(data->surname, "%d", i);

        hashTable = HashTableInsert(hashTable, data);
    }
    return 0;
}
