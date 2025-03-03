#include <stdlib.h>
#include <stdio.h>

typedef struct {
    int first;
    int second;
} inp;

int BinarySearch (inp * arr, int low, int height, inp found){
    if (low > height) return -1;


    int tmp = (height + low)/2;

    if (arr[tmp].first == found.first && arr[tmp].second == found.second) return tmp;
    if (found.first < arr[tmp].first){
        return BinarySearch(arr, low, tmp-1, found);
    } else if (found.first > arr[tmp].first){
        return BinarySearch(arr, tmp+1, height, found);
    } else if (found.second > arr[tmp].second){
        return BinarySearch(arr, tmp+1, height, found);
    } else if (found.second < arr[tmp].second){
        return BinarySearch(arr, low, tmp-1, found);
    }
    return -1;
}

// char ClearStr(char *str, int length) {
//     int i, j = 0;
//     char ExistChar = 0;
//     for (i = 0; i < length; i++) {
//         if (str[i] == ',')
//             ExistChar = 1;
//         if (str[i] != ' ') {
//         // if (str[i] >= '0' && str[i] <= '9' || str[i] == ',' || str[i] == '.'){
//             str[j++] = str[i];
//         }
//     }
//     str[j] = '\0';
//     return ExistChar;
// }

void getDataFromString (inp* input, char *inputString){
    if(*inputString == '-'){
        sscanf(inputString, "%d,%d", &(*input).first, &(*input).second);
        (*input).second *=-1;
    }
    else{
        sscanf(inputString, "%d", &(*input).first);
        (*input).second = 0;
    }

    while (((*input).second % 10) == 0 && (*input).second !=0){
        (*input).second /= 10;
    }
    // printf("%s", inputString);
}


int main(){
    inp * arr = NULL;
    int N = 0;
    int M = 0;
    char inputString[30] = "";

    scanf ("%d", &N);
    getchar();
    arr = (inp*)malloc (N*sizeof(inp));

    for (int i = 0; i < N; i++){
        fgets(inputString, sizeof(inputString), stdin);
        getDataFromString(&arr[i], inputString);
    }


    scanf("%d", &M);
    getchar();
    int * result = (int*)calloc (M, sizeof(int));
    inp tmp;
    for (int i = 0; i < M; i++){
        fgets(inputString, sizeof(inputString), stdin);
        getDataFromString(&tmp, inputString);

        result[i] = BinarySearch (arr, 0, N, tmp)+1;
    }
    for (int i = 0; i < M; i++){
        if ((i+1) == M)
            printf("%d", result[i]);
        else
            printf("%d\n", result[i]);
    }
    return 0;
}