#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "stack.h"

Stack* stack_new(void) {
    Stack* stack_p = malloc(sizeof(Stack));
    if (stack_p != NULL) {
        stack_p->head_p = NULL;
    }
    return stack_p;
}

bool stack_empty(const Stack* stack_p) { return stack_p->head_p == NULL; }

void* stack_peek(const Stack* stack_p) { return stack_p->head_p->value_p; }

bool stack_push(Stack* stack_p, void* value_p) {
    StackElement* elm_p = malloc(sizeof(StackElement));
    if (elm_p == NULL) {
        return false;
    }
    elm_p->value_p = value_p;
    elm_p->next_p = stack_p->head_p;
    stack_p->head_p = elm_p;
    return true;
}

void* stack_pop(Stack* stack_p) {
    void* value_p = stack_p->head_p->value_p;
    StackElement* next_p = stack_p->head_p->next_p;
    free(stack_p->head_p);
    stack_p->head_p = next_p;
    return value_p;
}

void stack_clear(Stack* stack_p) {
    StackElement* l = stack_p->head_p;
    StackElement* r = NULL;
    while (l != NULL) {
        r = l->next_p;
        free(l->value_p);
        free(l);
        l = r;
    }
    stack_p->head_p = NULL;
}

void stack_free(Stack* stack_p) {
    stack_clear(stack_p);
    free(stack_p);
}
