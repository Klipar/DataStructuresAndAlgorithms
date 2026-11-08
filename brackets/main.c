#include <stdlib.h>
#include <stdio.h>

int test (char *buffer){
    char ch;
    buffer[0] = '0';
    int i = 0;
    int error = 0;
    while (1){
        ch = getchar();

        if ((ch == '\n' || ch == EOF)) break;

        if (ch == '('|| ch == '['|| ch == '<'|| ch == '{'){
                i++;
                buffer[i] = ch;
                continue;
            }

        if (i == 0 && (ch != ')' || ch != ']' || ch != '>' || ch != '}')){
            error++;
            continue;
        }

        if (buffer[i] == '(' && ch != ')' || buffer[i] == '[' && ch != ']' || buffer[i] == '<' && ch != '>' || buffer[i] == '{' && ch != '}'  )
            error++;

        buffer[i] = '\0';
        if (i > 0)
            i--;

    }
    return error + i;
}

int main(){
    char * buffer = (char*)malloc (sizeof(char)*100000);
    int N;
    scanf("%d", &N);
    getchar();
    int * errors = (int*)malloc (sizeof(int)*N);

    for (int i = 0; i < N; i++)
        errors[i] = test(buffer);

    for (int i = 0; i < N; i++){
        if (i+1 == N)
            printf("%d", errors[i]);
        else
            printf("%d\n", errors[i]);
    }

    free (buffer);
    free (errors);
    return 0;
}
