#include <stdlib.h>
#include <stdio.h>

typedef struct inp {
    unsigned long first;
    unsigned long second;
    unsigned long GreatestCommonDivisor;
} inp;

void Swap (inp *data){
    unsigned tmp = (*data).first;
    (*data).first = (*data).second;
    (*data).second = tmp;
}

void GreatestCommonDivisor (inp *data){
    unsigned long i = 0;
    if ((*data).first > (*data).second)
        Swap(data);

    (*data).GreatestCommonDivisor = (*data).first;

    while (1){
        if (! ((*data).first % (*data).GreatestCommonDivisor || (*data).second % (*data).GreatestCommonDivisor))
            return;
        i++;
        while ((*data).first % i)
            i++;
        (*data).GreatestCommonDivisor = (*data).first / i;
    }
    return;
}

int main(){
    int counter = 0;
    inp* input = NULL;
    int i = 0;

    scanf ("%d", &counter);
    input = (inp*)malloc (sizeof(inp)*counter);

    for (i = 0; i < counter; i++){
        scanf("%lu %lu", &input[i].first, &input[i].second);
        GreatestCommonDivisor(&(input[i]));
    }
    for (i = 0; i < counter; i++){
        printf("%lu", input[i].GreatestCommonDivisor);
        if (i != (counter-1))
            putchar('\n');
    }
    free (input);
    return 0;
}
