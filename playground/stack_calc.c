#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

int stack[MAX_SIZE];
int top = -1;

void push(int x) {
    if (top == MAX_SIZE - 1) {
        printf("Error: stack overflow\n");
        return;
    }
    stack[++top] = x;
}

int pop() {
    if (top == -1) {
        printf("Error: No element to pop\n");
        return -1;
    }
    return stack[top--];
}

int main() {
    char expression[] = "AB*CD*+";
    int values[] = { 1, 2, 3, 4 };
    int len = strlen(expression);

    for (int i = 0; i < len; i++) {
        if (expression[i] >= 'A' && expression[i] <= 'D') {
            push(values[expression[i] - 'A']);
        }
        else {
            int op1 = pop();
            int op2 = pop();

            switch (expression[i]) {
            case '+': push(op2 + op1); break;
            case '-': push(op2 - op1); break;
            case '*': push(op2 * op1); break;
            case '/': push(op2 / op1); break;
            }
        }
    }

    printf("Result: %d\n", pop());

    return 0;
}