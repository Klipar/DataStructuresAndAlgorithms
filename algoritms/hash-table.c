#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define LOADFACTOR 0.75     // [0-1]
#define DEFAULT_SIZE 10     // [0-+∞]
#define PRIME_NUMBER 19     // Prime number for the second hash function

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

bool HashTableIsDataExist(Data* data){
    return !(data->ID == 0 && data->name[0] == '\0' && data->surname[0] == '\0');
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
        printf("arr id\tID\tName\tSurname\n");
        bool skip = false;
        for (int i = 0; i < hashTable->size; i++){
            if (HashTableIsDataExist(hashTable->arr+i)){
                if (skip)
                    skip = false;
                printf("[%0*d] ID: %u\t %s \t %s\n", count, i, (hashTable->arr+i)->ID, (hashTable->arr+i)->name, (hashTable->arr+i)->surname);
            } else if (skip == false){
                    skip = true;
                    printf("\t\t.\n\t\t.\n\t\t.\n");

            }
        }
    }
}


/*   index = (h1(key) + i · h2(key)) mod arrSize,
 *   h1(key) = key mod arrSize
 *   h2(key) = primeNumber - (key mod primeNumber)
 */
unsigned int HashTableGetArrId(HashTable* hashTable, unsigned int ID){
    if (hashTable == NULL)
        return 0;

    unsigned int hash1 = ID % hashTable->size;
    unsigned int arrID;
    for (unsigned int i = 0; true ; i++){
        arrID = (hash1 + i*(PRIME_NUMBER - (ID % PRIME_NUMBER))) % hashTable->size;
        if ((hashTable->arr+arrID)->ID == ID || !HashTableIsDataExist(hashTable->arr+arrID))
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
            if  (HashTableIsDataExist(hashTable->arr+i))
                newTable = HashTableInsert(newTable, hashTable->arr[i]);

        HashTableFree(hashTable);
        hashTable = newTable;
    }

    unsigned int arrID = HashTableGetArrId(hashTable, data.ID);
    if (!HashTableIsDataExist(hashTable->arr+arrID))
        hashTable->elementCount++;
    hashTable->arr[arrID] = data;

    return hashTable;
}

Data* HashTableSearch (HashTable* hashTable, unsigned int ID){
    if (hashTable == NULL)
        return NULL;
    Data* data = hashTable->arr+HashTableGetArrId(hashTable, ID);
    if (HashTableIsDataExist(data))
        return data;
    return NULL;
}

void HashTableDelete (HashTable* hashTable, unsigned int ID){
    Data* data = HashTableSearch(hashTable, ID);
    if (data != NULL){
        data->ID = 0;
        data->name[0] = '\0';
        data->surname[0] = '\0';
        hashTable->elementCount--;
    }
}

int main(){
    HashTable* hashTable = HashTableInit(50, 0.75);

    for (int i = 0; i <= 50; i++){
        Data data;
        data.ID = i;

        sprintf(data.name, "%d", i);
        sprintf(data.surname, "%d", i);

        hashTable = HashTableInsert(hashTable, data);
    }

    HasTablePrint(hashTable);
    for (int i = 0; i <= 50; i++){
        HashTableDelete(hashTable, i);
    }
    HasTablePrint(hashTable);
    HashTableFree(hashTable);
    return 0;
}
