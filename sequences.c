#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    int initial = atoi(argv[1]);

    //array of operators 
    int size = 4;
    char **operator = malloc(size * sizeof(char *));
    int len = 0;

    //array of operands
    int size2 = 4;
    int *opearands = malloc(size2 * sizeof(int));
    int len2 = 0;

    char next[50] = '';
    
    while(scanf("%s", next) == 1){

        if(strcmp(next, "n") == 0){
            //apply all operations to current, print on newline
        }
        
        else {
            operator[len] = next;
            int operation = 0;
            operation = scanf(%d, operation);
            operands[len2] = operation;
            len++;
            len2++;
        }

        if(len == size){
            
            char **new_op = malloc((size*2) * sizeof(char *));
            for(int i = 0; i < size; i++){
                new_op[i] = operator[i];
            }
            size = size*2;
            free(operator);
            operator = new_op;

            int *new_operands = malloc((size2*2) * sizeof(int));
            for(int i = 0; i < size2; i++){
                new_operands[i] = operands[i];
            }
            size2 = size2*2;
            free(operands);
            operands = new_operands;
        }

    }


}