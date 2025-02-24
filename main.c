#include <stdlib.h>
#include <stdio.h>

int main(){
    int N = 0, M = 0;

    scanf ("%d %d", &N, &M);

    char* arr = (char*)malloc(M*sizeof(char)+1);

    arr[0] = arr[1] = 0;

    for (int i = 2; i <= M; i++) 
        arr[i] = 1;

    for (int i = 0; i <= M; i++) 
        if (arr[i])
            for (int j = 2; i*j <= M; j++)
                arr[i*j] = 0;

    for (int i = N; i <= M; i++)
        if (arr[i])
            (N == -1) ? printf(", %d", i) : (printf("%d", i), (N = -1));

    free (arr);

    return 0;
}