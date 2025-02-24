#include <stdlib.h>
#include <stdio.h>

int main(){
    unsigned int N = 0;
    unsigned int  M = 0;

    char* arr = NULL;

    scanf ("%u %u", &N, &M);

    arr = (char*)malloc(((M*sizeof(char))+1));

    arr[0] = arr[1] = 0;

    for (int i = 2; i <= M; i++) {
        arr[i] = 1;
    }

    // for (int i = 0; i <= M; i++) printf("%d ", arr[i]);
    // putchar('\n');

    for (int i = 0; i <= M; i++) {
        if (arr[i] == 1){
            for (int j = 2; i*j <= M; j++)
                arr[i*j] = 0;
        }
    }
    char tnp = 0;
    
    for (int i = N; i <= M; i++){
        if (arr[i]){
            if (tnp){
                printf(", %d", i);
            } else {
                printf("%d", i);
                tnp++;
            }
        }
    }

    free (arr);
    return 0;
}