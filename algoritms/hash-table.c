#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define LOADFACTOR 0.75     // [0-1]
#define DEFAULT_SIZE 10     // [0-+∞]
#define PRIME_NUMBER 19     // Prime number for the second hash function

#define DATA_NOT_EXIST 1000000000
#define DATA_DELETED   1000000001

enum TypeOfGettingElement {
    Insert,
    SearchAndDelete
};

typedef enum TypeOfGettingElement GetIdType;

#if !defined(DATA)
#define DATA
typedef struct Data {
    unsigned int ID;
    char name [20];
    char surname [20];
} Data;
#endif // DATA

typedef struct HashTable {
    Data* arr;
    unsigned size;
    unsigned elementCount;
    float loadFactor;
} HashTable;

HashTable* HashTableInit(unsigned int size, float loadFactor){
    HashTable* table = malloc (sizeof(HashTable));

    if (size <= 0 || loadFactor > 1 || loadFactor <= 0)
        return NULL;

    table->size = size;
    table->arr = malloc(table->size*sizeof(Data));
    for (unsigned int i = 0; i < table->size; i++)
        table->arr[i].ID = DATA_NOT_EXIST;

    table->elementCount = 0;
    table->loadFactor = loadFactor;
    return table;
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
        printf("arr id\tID\tName\t\t\tSurname\n");
        bool skip = false;
        for (int i = 0; i < hashTable->size; i++){
            if (hashTable->arr[i].ID != DATA_DELETED && hashTable->arr[i].ID != DATA_NOT_EXIST){
                if (skip)
                    skip = false;
                printf("[%0*d] ID: %u\t %-20s\t%s\n", count, i, hashTable->arr[i].ID, hashTable->arr[i].name, hashTable->arr[i].surname);
            } else if (skip == false){
                    skip = true;
                    printf("\t\t.\n\t\t.\n\t\t.\n");

            }
        }
    }
}

unsigned int HashTableGetArrId(HashTable* hashTable, unsigned int ID, GetIdType type){
    if (hashTable == NULL)
        return 0;

    unsigned int hash1 = ID % hashTable->size;
    unsigned int hash2 = 1 + (ID % (hashTable->size-1));
    unsigned int arrID;
    for (unsigned int i = 0; true ; i++){
        arrID = (hash1 + i*hash2) % hashTable->size;
        if  ((hashTable->arr[arrID].ID == ID) ||
            (type == SearchAndDelete && hashTable->arr[arrID].ID == DATA_NOT_EXIST) ||
            (type == Insert && (hashTable->arr[arrID].ID == DATA_NOT_EXIST || hashTable->arr[arrID].ID ==DATA_DELETED)))
            return arrID;
    }
}

void HashTableFree (HashTable* hashTable){
    if (hashTable == NULL)
        return;

    if (hashTable->size > 0)
        free(hashTable->arr);
    free(hashTable);
}

HashTable* HashTableInsert (HashTable* hashTable, Data data){
    if (hashTable == NULL)
        hashTable = HashTableInit(DEFAULT_SIZE, LOADFACTOR);

    if ((float)hashTable->elementCount/(float)hashTable->size >= LOADFACTOR){
        HashTable* newTable = HashTableInit(hashTable->size*2, LOADFACTOR);

        for (int i = 0; i < hashTable->size; i++) //copy all previous data
            if  (hashTable->arr[i].ID != DATA_NOT_EXIST && hashTable->arr[i].ID != DATA_DELETED)
                newTable = HashTableInsert(newTable, hashTable->arr[i]);

        HashTableFree(hashTable);
        hashTable = newTable;
    }

    unsigned int arrID = HashTableGetArrId(hashTable, data.ID, Insert);
    if (hashTable->arr[arrID].ID == DATA_NOT_EXIST || hashTable->arr[arrID].ID == DATA_DELETED)
        hashTable->elementCount++;
    hashTable->arr[arrID] = data;

    return hashTable;
}

Data* HashTableSearch (HashTable* hashTable, unsigned int ID){
    if (hashTable == NULL)
        return NULL;
    Data* data = hashTable->arr+HashTableGetArrId(hashTable, ID, SearchAndDelete);
    if (data->ID != DATA_NOT_EXIST && data->ID != DATA_DELETED)
        return data;
    return NULL;
}

void HashTableDelete (HashTable* hashTable, unsigned int ID){
    Data* data = HashTableSearch(hashTable, ID);
    if (data != NULL)
        data->ID = DATA_DELETED;
}

// #if !defined(WORK_MODE)
// int main(){
//     HashTable* hashTable = HashTableInit(50, 0.75);

//     for (int i = 0; i <= 50; i++){
//         Data data;
//         data.ID = i;

//         sprintf(data.name, "%d", i);
//         sprintf(data.surname, "%d", i);

//         hashTable = HashTableInsert(hashTable, data);
//     }

//     HasTablePrint(hashTable);
//     for (int i = 0; i <= 50; i++){
//         HashTableDelete(hashTable, i);
//     }
//     HasTablePrint(hashTable);
//     HashTableFree(hashTable);
//     return 0;
// }
// #endif
