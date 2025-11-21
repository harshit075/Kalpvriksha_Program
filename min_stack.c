#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int *stack;
    int *minStack;
    int top;
    int size;
} MinStack;

MinStack* minStackCreate() {
    MinStack* obj = (MinStack*)malloc(sizeof(MinStack));
    obj->size = 100000; // large enough capacity
    obj->stack = (int*)malloc(sizeof(int) * obj->size);
    obj->minStack = (int*)malloc(sizeof(int) * obj->size);
    obj->top = -1;
    return obj;
}

void minStackPush(MinStack* obj, int val) {
    obj->top++;
    obj->stack[obj->top] = val;

    if (obj->top == 0)
        obj->minStack[obj->top] = val;
    else {
        int currentMin = obj->minStack[obj->top - 1];
        obj->minStack[obj->top] = (val < currentMin) ? val : currentMin;
    }
}

void minStackPop(MinStack* obj) {
    if (obj->top == -1) return;
    obj->top--;
}

int minStackTop(MinStack* obj) {
    return obj->stack[obj->top];
}

int minStackGetMin(MinStack* obj) {
    return obj->minStack[obj->top];
}

void minStackFree(MinStack* obj) {
    free(obj->stack);
    free(obj->minStack);
    free(obj);
}
