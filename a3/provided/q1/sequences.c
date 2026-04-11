#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Linked list node structure    
struct operationPair {
    char operator;  // maps: 1=add, 2=sub, 3=mul, 4=div
    int operand; // the integer value following the operator 
    struct operationPair *next; //pointer to next node 
};

//Main linked list structure 
struct head {
    int len;
    struct operationPair *head; // pointer to first node 
};


void appendOperation(struct head *list, char operator, int operand) {
    struct operationPair *node = malloc(sizeof(struct operationPair));
    node->next = NULL;
    node->operator = operator;
    node->operand = operand;

    // if this is the first node and the list is empty
    if (list->head == NULL) {
        list->head = node;
        list->len = 1;
    }

    //if the list already has elements
    else {
        struct operationPair *currentNode = list->head;
        while (currentNode->next != NULL) currentNode = currentNode->next;
        currentNode->next = node;
        list->len += 1;
    }
}

int applyOperations(struct head *list, int startValue) {
    int result = startValue;
    struct operationPair *currentNode = list->head;

    for (int i = 0; i < list->len; i++, currentNode = currentNode->next) {
        if (currentNode->operator == 1){
            result = result + currentNode->operand;
        }      
        else if (currentNode->operator == 2){
            result = result - currentNode->operand;
        }
        else if (currentNode->operator == 3){
            result = result * currentNode->operand;
        }
        else{
            result = result/currentNode->operand;
        }
    }
    return result;
}

//cleaning node by recursion
void freeOperationNodes(struct operationPair *node) {
    if (!node){
        return;
    }
    freeOperationNodes(node->next);
    free(node);
}

void freeOperationList(struct head *list) {
    if (!list){
        return;
    }
    freeOperationNodes(list->head);
    free(list);
}

// maps operator string to numerical codes for easier types 
char mapOperationCode(const char *operatorString) {
    if (strcmp(operatorString, "add") == 0){
        return 1;
    }
    if (strcmp(operatorString, "sub") == 0){
        return 2;
    }
    if (strcmp(operatorString, "mul") == 0){
        return 3;
    }
    if (strcmp(operatorString, "div") == 0){
        return 4;
    }
    return 0;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s N", argv[0]);
        return 1; //error 
    }

    int initial = atoi(*(argv + 1));

    struct head *list = malloc(sizeof(struct head));
    if (!list){
        return 1;
    }
    list->head = NULL;
    list->len = 0;

    char command[10];
    int operand;

    while (scanf("%9s", command) != EOF) {
        //operators are given
        if (strcmp(command, "n") != 0) {
            char operationCode = mapOperationCode(command);
            //second scanf to immediately pick the operand as well 
            if (scanf("%d", &operand) == 1 && operationCode != 0) {
                appendOperation(list, operationCode, operand);
            }
        } else { // n is given
            initial = applyOperations(list, initial);
            printf("%d\n", initial);
        }
    }

    freeOperationList(list);
    return 0;
}