#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int precedence(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*')
        return 2;
    return 0;
}

char* parseExpression(char* exp_input, char* exp_postfix, int* matrix_count) {
    int i, k;
    char operation_stack[strlen(exp_input)];
    int top = -1;

    for (i = 0, k = -1; exp_input[i]; ++i) {
        if (exp_input[i] >= 'A' && exp_input[i] <= 'Z') {
            exp_postfix[++k] = exp_input[i];
            (*matrix_count) += 1;
        }
        else {
            while (top != -1 && precedence(exp_input[i]) <= precedence(operation_stack[top]))
                exp_postfix[++k] = operation_stack[top--];
            operation_stack[++top] = exp_input[i];
        }
    }

    while (top != -1)
        exp_postfix[++k] = operation_stack[top--];

    exp_postfix[++k] = '\0';
}

int main() {
    // first line parse
    char exp_input[100];
    scanf("%s", &exp_input);

    int exp_len = strlen(exp_input);
    char exp_postfix[exp_len + 1];

    int matrix_count = 0;
    parseExpression(exp_input, exp_postfix, &matrix_count);


    // printf("%s -> %s (# of mat: %d)", exp_input, exp_postfix, matrix_count);

    for (int i = 0; i < matrix_count; i++) {
        int r, c;
        scanf("%d\t%d", &r, &c);
        // Matrix* matrix = init_matrix(r, c);
    }


    return 0;
}