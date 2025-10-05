#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX 100

int main() {
    char expression[MAX];
    printf("Enter expression: ");
    fgets(expression, MAX, stdin);

    int operands[MAX];
    char operators[MAX];
    int operandCount = 0, operatorCount = 0;

    int exprIndex = 0;
    while (expression[exprIndex]) {
        if (isspace(expression[exprIndex])) {
            exprIndex++;
            continue;
        }

        if (isdigit(expression[exprIndex])) {
            int value = 0;
            while (isdigit(expression[exprIndex])) {
                value = value * 10 + (expression[exprIndex] - '0');
                exprIndex++;
            }
            operands[operandCount++] = value;
        }
        else if (expression[exprIndex] == '+' || expression[exprIndex] == '-' ||
                 expression[exprIndex] == '*' || expression[exprIndex] == '/') {
            operators[operatorCount++] = expression[exprIndex];
            exprIndex++;
        }
        else if (expression[exprIndex] == '\n') {
            break; 
        }
        else {
            printf("Error: Invalid expression.\n");
            return 0;
        }
    }

  
    for (int opIndex = 0; opIndex < operatorCount; opIndex++) {
        if (operators[opIndex] == '*' || operators[opIndex] == '/') {
            if (operators[opIndex] == '*') {
                operands[opIndex] = operands[opIndex] * operands[opIndex + 1];
            } else {
                if (operands[opIndex + 1] == 0) {
                    printf("Error: Division by zero.\n");
                    return 0;
                }
                operands[opIndex] = operands[opIndex] / operands[opIndex + 1];
            }

            
            for (int shiftIndex = opIndex + 1; shiftIndex < operandCount - 1; shiftIndex++)
                operands[shiftIndex] = operands[shiftIndex + 1];
            for (int shiftIndex = opIndex; shiftIndex < operatorCount - 1; shiftIndex++)
                operators[shiftIndex] = operators[shiftIndex + 1];

            operandCount--;
            operatorCount--;
            opIndex--;
        }
    }

   
    int result = operands[0];
    for (int opIndex = 0; opIndex < operatorCount; opIndex++) {
        if (operators[opIndex] == '+') 
            result += operands[opIndex + 1];
        else if (operators[opIndex] == '-') 
            result -= operands[opIndex + 1];
    }

    printf("Result: %d\n", result);
    return 0;
}
