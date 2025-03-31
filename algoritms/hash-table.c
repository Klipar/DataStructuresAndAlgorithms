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
        for (int i = 0; i < hashTable->size; i++){
            printf("[%0*d] ID: %u\t %s \t %s\n", count, i, (hashTable->arr+i)->ID, (hashTable->arr+i)->name, (hashTable->arr+i)->surname);
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
        if ((hashTable->arr+arrID)->ID == ID ||
            ((hashTable->arr+arrID)->ID == 0 && (hashTable->arr+arrID)->name[0] == '\0' && (hashTable->arr+arrID)->surname[0] == '\0')){
                return arrID;
        }
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
            if  (!((hashTable->arr+i)->ID == 0 && (hashTable->arr+i)->name[0] == '\0' && (hashTable->arr+i)->surname[0] == '\0'))
                newTable = HashTableInsert(newTable, hashTable->arr[i]);

        HashTableFree(hashTable);
        hashTable = newTable;
    }

    unsigned int arrID = HashTableGetArrId(hashTable, data.ID);
    if ((hashTable->arr+arrID)->ID == 0 && (hashTable->arr+arrID)->name[0] == '\0' && (hashTable->arr+arrID)->surname[0] == '\0')
        hashTable->elementCount++;
    hashTable->arr[arrID] = data;

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
