#include <stdio.h>
#include <time.h>
#include "algoritms/2-3-tree.c"
#include "algoritms/red-black-tree.c"
#include "algoritms/hash-table.c"
// #define WORK_MODE

typedef struct TestAlgorithms {
    clock_t InsertTimer;
    clock_t SearchTimer;
    clock_t DeleteTimer;
    unsigned int MaxMemoryUse;
} TestAlgorithms;

typedef struct TestGlobal {
    unsigned int InsertCounter;
    unsigned int SearchCounter;
    unsigned int DeleteCounter;
} TestGlobal;

void printTestAlgorithm(TestAlgorithms testAlgorithm, TestGlobal testGlobal, char* algorithmName) {
    double insertTimeMs = (double)testAlgorithm.InsertTimer / CLOCKS_PER_SEC * 1000.0;
    double searchTimeMs = (double)testAlgorithm.SearchTimer / CLOCKS_PER_SEC * 1000.0;
    double deleteTimeMs = (double)testAlgorithm.DeleteTimer / CLOCKS_PER_SEC * 1000.0;

    unsigned int insertCount = testGlobal.InsertCounter;
    unsigned int searchCount = testGlobal.SearchCounter;
    unsigned int deleteCount = testGlobal.DeleteCounter;

    double avgInsert = insertCount > 0 ? insertTimeMs / insertCount : 0.0;
    double avgSearch = searchCount > 0 ? searchTimeMs / searchCount : 0.0;
    double avgDelete = deleteCount > 0 ? deleteTimeMs / deleteCount : 0.0;

    printf("\nAlgorithm: %s\n", algorithmName);

    printf("+-------------+--------------+-------------+----------------+\n");
    printf("| Operation   | Time (ms)    | Operations  | Avg Time (ms)  |\n");
    printf("+-------------+--------------+-------------+----------------+\n");

    printf("| Insert      | %12.2f | %11u | %14.4f |\n", insertTimeMs, insertCount, avgInsert);
    printf("| Search      | %12.2f | %11u | %14.4f |\n", searchTimeMs, searchCount, avgSearch);
    printf("| Delete      | %12.2f | %11u | %14.4f |\n", deleteTimeMs, deleteCount, avgDelete);

    printf("+-------------+--------------+-------------+----------------+\n");

    printf("Max Memory Used by Algorithm: %u KB\n", testAlgorithm.MaxMemoryUse / 1024);
}


void testFromFile (const char* filePath){
    FILE *file;
    char line[100];
    bool IsCountedFor23tree = false;
    clock_t timeStump;

    HashTable* hashTable = HashTableInit(50, 0.70);
    Tree_2_3 * tree_2_3 = NULL;
    RedBlackTree* redBlackTree = NULL;

    TestGlobal test = {};

    TestAlgorithms testHashTable = {};
    TestAlgorithms testTree_2_3 = {};
    TestAlgorithms testRedBlackTree = {};

    file = fopen(filePath, "r");

    if (file == NULL) {
        printf("File could not be opened.\n");
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] == 'i'){
            Data data;

            if (sscanf(line, "i %d %[^ ] %[^\n]", &data.ID, data.name, data.surname) != 3) {
                printf("pars failed while parsing string:\n%s", line);
                return;
            }

            test.InsertCounter++;

            timeStump = clock();
            hashTable = HashTableInsert(hashTable, data);
            testHashTable.InsertTimer += clock() - timeStump;
            unsigned int tmpByte = hashTable->size*sizeof(Data)+sizeof(HashTable);
            if (tmpByte > testHashTable.MaxMemoryUse)
                testHashTable.MaxMemoryUse = tmpByte;


            timeStump = clock();
            tree_2_3 = Tree_2_3_Insert(tree_2_3, data);
            testTree_2_3.InsertTimer += clock() - timeStump;

            timeStump = clock();
            redBlackTree = RedBlackTreeInsert(redBlackTree, data);
            testRedBlackTree.InsertTimer += clock() - timeStump;
            testRedBlackTree.MaxMemoryUse += sizeof(RedBlackTree);

        } else if (line[0] == 's'){
            if (!IsCountedFor23tree){
                testTree_2_3.MaxMemoryUse = sizeof(Tree_2_3)*countNodes(tree_2_3);
                IsCountedFor23tree = true;
            }

            unsigned int ID;

            if (sscanf(line, "s %d", &ID) != 1) {
                printf("pars failed while parsing string:\n%s", line);
                return;
            }

            test.SearchCounter++;

            Data* tmpHashTable;
            timeStump = clock();
            tmpHashTable = HashTableSearch(hashTable, ID);
            testHashTable.SearchTimer += clock() - timeStump;

            if (tmpHashTable == NULL){
                printf("Search HashTable failed, for ID %d\n", ID);
                return;
            }

            Tree_2_3* tmpTree_2_3;
            timeStump = clock();
            tmpTree_2_3 = Tree_2_3_Search(tree_2_3, ID);
            testTree_2_3.SearchTimer += clock() - timeStump;

            if (tmpTree_2_3 == NULL){
                printf("Search Tree_2_3 failed, for ID %d\n", ID);
                return;
            }

            Data* tmpRedBlackTree;
            timeStump = clock();
            tmpRedBlackTree = RedBlackTreeSearch(redBlackTree, ID);
            testRedBlackTree.SearchTimer += clock() - timeStump;

            if (tmpRedBlackTree == NULL){
                printf("Search RedBlackTree failed, for ID %d\n", ID);
                return;
            }

        } else if (line[0] == 'r'){
            unsigned int ID;

            if (sscanf(line, "r %d", &ID) != 1) {
                printf("pars failed while parsing string:\n%s", line);
                return;
            }

            test.DeleteCounter++;

            timeStump = clock();
            HashTableDelete(hashTable, ID);
            testHashTable.DeleteTimer += clock() - timeStump;

            timeStump = clock();
            tree_2_3 = Tree_2_3_Delete(tree_2_3, ID);
            testTree_2_3.DeleteTimer += clock() - timeStump;

            timeStump = clock();
            redBlackTree = RedBlackTreeDelete(redBlackTree, ID);
            testRedBlackTree.DeleteTimer += clock() - timeStump;
        }
    }

    fclose(file);
    HashTableFree(hashTable);
    Tree_2_3_Free(tree_2_3);
    RedBlackTreeFree(redBlackTree);

    printTestAlgorithm(testHashTable, test, "Hash table");
    printTestAlgorithm(testTree_2_3, test, "2-3 tree");
    printTestAlgorithm(testRedBlackTree, test, "Red-black tree");
}

int main(){
    testFromFile("tests/tests/test_10.txt");
    testFromFile("tests/tests/test_100.txt");
    testFromFile("tests/tests/test_1000.txt");
    testFromFile("tests/tests/test_10000.txt");
    testFromFile("tests/tests/test_100000.txt");
    testFromFile("tests/tests/test_1000000.txt");
    testFromFile("tests/tests/test_10000000.txt");
    return 0;
}