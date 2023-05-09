#include "stack.h"

struct stack {
    elem_t *stack_elements;
    int size;
    int max_size;
    clone_t elem_clone;
    destroy_t elem_destroy;
    print_t elem_print;
};

pstack_t stack_create(size_t max_num_of_elem,
                      clone_t elem_clone,
                      destroy_t elem_destroy,
                      print_t elem_print) {
    pstack_t new_stack = malloc(sizeof(pstack_t));
    if (new_stack == NULL) {
        return NULL;
    }
    new_stack->stack_elements = malloc(sizeof(elem_t) * max_num_of_elem);
    if (new_stack->stack_elements == NULL) {
        return NULL;
    }
    new_stack->size = 0;
    new_stack->max_size = max_num_of_elem;
    new_stack->elem_clone = elem_clone;
    new_stack->elem_destroy = elem_destroy;
    new_stack->elem_print = elem_print;
    return new_stack;
}

Result stack_destroy(pstack_t stack) {
    if (stack == NULL) {
        return FAIL;
    }
    while (!stack_is_empty(stack)) {
        stack_pop(stack);
    }
    free(stack->stack_elements);
    free(stack);
    return SUCCESS;
}

Result stack_push(pstack_t stack, elem_t new_elem) {
    if ((stack == NULL) && (stack->size >= stack->max_size)) {
        return FAIL;
    }
    stack->stack_elements[stack->size++] = stack->elem_clone(new_elem);
    return SUCCESS;
}

void stack_pop(pstack_t stack) {
    if (stack == NULL) {
        return;
    }
    stack->elem_destroy(stack->stack_elements[stack->size]);
    stack->size--;
}

elem_t stack_peek(pstack_t stack) {
    if (stack == NULL) {
        return NULL;
    }
    return stack->stack_elements[stack->size];
}

size_t stack_size(pstack_t stack) {
    if (stack == NULL) {
        return 0;
    }
    return stack->size;
}

bool stack_is_empty(pstack_t stack) {
    if (stack == NULL) {
        return 0;
    }
    return stack->size == 0;
}

int stack_capacity(pstack_t stack) {
    return (stack->max_size - stack->size);
}

void stack_print(pstack_t stack) {
    for (int i = stack->size - 1; i >= 0; i--) {
        stack->elem_print(stack->stack_elements[i]);
    }
}