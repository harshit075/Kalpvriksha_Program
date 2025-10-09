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

    
    for (int operatorIndex = 0; operatorIndex < operatorCount; operatorIndex++) {
        if (operators[operatorIndex] == '*' || operators[operatorIndex] == '/') {
            if (operators[operatorIndex] == '*') {
                operands[operatorIndex] = operands[operatorIndex] * operands[operatorIndex + 1];
            } else {
                if (operands[operatorIndex + 1] == 0) {
                    printf("Error: Division by zero.\n");
                    return 0;
                }
                operands[operatorIndex] = operands[operatorIndex] / operands[operatorIndex + 1];
            }

            
            for (int shiftIndex = operatorIndex + 1; shiftIndex < operandCount - 1; shiftIndex++)
                operands[shiftIndex] = operands[shiftIndex + 1];
            for (int shiftIndex = operatorIndex; shiftIndex < operatorCount - 1; shiftIndex++)
                operators[shiftIndex] = operators[shiftIndex + 1];

            operandCount--;
            operatorCount--;
            operatorIndex--; 
        }
    }

   
    int result = operands[0];
    for (int operatorIndex = 0; operatorIndex < operatorCount; operatorIndex++) {
        if (operators[operatorIndex] == '+') 
            result += operands[operatorIndex + 1];
        else if (operators[operatorIndex] == '-') 
            result -= operands[operatorIndex + 1];
    }

    printf("Result: %d\n", result);
    return 0;
}
