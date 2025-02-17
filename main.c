#include <stdlib.h>
#include <stdio.h>

typedef struct {
    unsigned long first;
    unsigned long second;
    unsigned long GreatestCommonDivisor;
} inp;

int GreatestCommonDivisor (inp data){
    while (data.first && data.second){
        if (data.first > data.second){
            data.first =  data.first % data.second;
        } else {
            data.second = data.second % data.first;
        }
    }
    if (data.first)
        return data.first;
    return data.second;
}

int main(){
    int counter = 0;
    inp* input = NULL;
    int i = 0;

    scanf ("%d", &counter);
    input = (inp*)malloc (sizeof(inp)*counter);

    for (i = 0; i < counter; i++){
        scanf("%lu %lu", &input[i].first, &input[i].second);
        input[i].GreatestCommonDivisor = GreatestCommonDivisor(input[i]);
    }
    for (i = 0; i < counter; i++){
        printf("%lu", input[i].GreatestCommonDivisor);
        if (i != (counter-1))
            putchar('\n');
    }
    free (input);
    return 0;
}
