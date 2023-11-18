#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

int stack[MAX_SIZE];
int top = -1;

int precedence(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*')
        return 2;
    return 0;
}

char* infixToPostfix(char* expression, char* calculation_queue) {
    int i, k;
    char operation_stack[strlen(expression)];
    int top = -1;

    for (i = 0, k = -1; expression[i]; ++i) {
        if (expression[i] >= 'A' && expression[i] <= 'Z')
            calculation_queue[++k] = expression[i];
        else {
            while (top != -1 && precedence(expression[i]) <= precedence(operation_stack[top]))
                calculation_queue[++k] = operation_stack[top--];
            operation_stack[++top] = expression[i];
        }
    }

    while (top != -1)
        calculation_queue[++k] = operation_stack[top--];

    calculation_queue[++k] = '\0';
}

int main() {
    char expression[] = "A*B+C*D";
    char calculation_queue[strlen(expression) + 1];
    infixToPostfix(expression, calculation_queue);
    printf("%s\n", calculation_queue); // "AB*CD*+"
    int a, b, c, d;
    scanf("%d %d %d %d", &a, &b, &c, &d);
    printf("%d, %d, %d, %d\n", a, b, c, d);
    printf("Expected: %d\n", a * b + c * d);


    int number_stack[4] = {a, b, c, d};
    int len = strlen(calculation_queue);

    for (int i = 0; i < len; i++) {
        if(calculation_queue[i] >= 'A' && calculation_queue[i] <= 'Z') {
            int idx = calculation_queue[i] - 'A';
            // push(number[idx])
            stack[++top] = number_stack[idx];
        } else {
            // pop two numbers
            int num1 = stack[top--];
            int num2 = stack[top--];
            // calculate
            int result;
            switch (calculation_queue[i]) {
                case '+':
                    result = num1 + num2;
                    break;
                case '-':
                    result = num1 - num2;
                    break;
                case '*':
                    result = num1 * num2;
                    break;
            }
            // push result
            stack[++top] = result;
        }
    }

    printf("Result: %d\n", stack[top--]);

    return 0;
}