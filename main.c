#include <stdlib.h>
#include <stdio.h>

// ( ), [ ], <>, or { }

char test (char *buffer){
    char ch = getchar();
    int i = 0;
    buffer[i] = ch;
    int error = 0;
    while (1){
        ch = getchar();
        if (ch == '\n' || ch == '\n'){
            if (buffer[i] != '\0')
                i++;
            return (error + i);
        }

        if (ch == '('||
            ch == '['||
            ch == '<'||
            ch == '{'){
                i++;
                buffer[i] = ch;
                continue;
            }
        if (buffer[i] == '\0' && (ch == ')'||
                        ch == ']'||
                        ch == '>'||
                        ch == '}'))
            error++;
        if (buffer[i] == '(' && ch != ')' ||
            buffer[i] == '[' && ch != ']' ||
            buffer[i] == '<' && ch != '>' ||
            buffer[i] == '{' && ch != '}'  ) {
                error++;
            }
        buffer[i] = '\0';
        if (i > 0){
            i--;
        }
    }
    return error + i;
}

int main(){
    char * buffer = (char*)malloc (sizeof(char)*100000);
    char N;
    scanf("%d", &N);
    getchar();
    char * errors = (char*)malloc (sizeof(char)*N);

    for (int i = 0; i < N; i++){
        errors[i] = test(buffer);
    }

    for (int i = 0; i < N; i++){
        if (i+1 == N)
            printf("%d", errors[i]);
        else
            printf("%d\n", errors[i]);
    }

    free (buffer);
    return 0;
}
