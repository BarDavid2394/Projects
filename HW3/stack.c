#include "stack.h"


//crearting a stack with the relevant types
pstack_t stack_create(size_t max_num_of_elem,
                      clone_t elem_clone,
                      destroy_t elem_destroy,
                      print_t elem_print) {
    //allocating space to the new stack
    pstack_t new_stack =(struct stack*)malloc(sizeof(struct stack));
    if (new_stack == NULL) {
        return NULL;
    }
    //allocating space for the array of elements inside the stack
    new_stack->stack_elements=(elem_t)malloc(sizeof(elem_t) * max_num_of_elem);
    if (new_stack->stack_elements == NULL) {
        return NULL;
    }
    new_stack->size = 0;
    new_stack->max_size = max_num_of_elem;
    new_stack->elem_clone = elem_clone;
    new_stack->elem_destroy = elem_destroy;
    new_stack->elem_print = elem_print;
    return (new_stack);
}
//The function destroys the stack with all the elements inside, returns result
Result stack_destroy(pstack_t stack) {
    if (stack == NULL) {
        return FAIL;
    }
    //as long as the stack is not empty, keep popping elements.
    while (!stack_is_empty(stack)) {
        stack_pop(stack);
    }
    //freeing the space we have allocated in stack_create
    free(stack->stack_elements);
    free(stack);
    return SUCCESS;
}
//inserting a new element to the top of the stack. returns result.
Result stack_push(pstack_t stack, elem_t new_elem) {
    //we check that the stack we got is valid and not full.
    if ((stack == NULL) || (stack->size >= stack->max_size)) {
        return FAIL;
    }
    //copying the element and inserting the copy.
    elem_t copy_elem = stack->elem_clone(new_elem);
    stack->stack_elements[stack->size++] = copy_elem;
    return SUCCESS;
}
//extracting an elements from the top of the stack.
void stack_pop(pstack_t stack) {
    //we check that the stack we got is valid and not empty.
    if (stack == NULL || stack_is_empty(stack)) {
        return;
    }
    stack->elem_destroy(stack->stack_elements[stack->size -1]);
    stack->size--;
}
//peeking in the top element in the stack.
elem_t stack_peek(pstack_t stack) {
    //we check that the stack we got is valid and not empty.
    if (stack == NULL || stack_is_empty(stack)) {
        printf("Error: Cannot peek an empty stack.");
        return NULL;
    }
    return stack->stack_elements[stack->size-1];
}
//returns the number of elements inside the stack
size_t stack_size(pstack_t stack) {
    if (stack == NULL) {
        return 0;
    }
    return stack->size;
}
//This function returns if true if the stack is empty. otherwise, false.
bool stack_is_empty(pstack_t stack) {
    if (stack == NULL) {
        return 0;
    }
    return stack->size == 0;
}
//this function returns the number of space left in the stack.
int stack_capacity(pstack_t stack) {
    return (stack->max_size - stack->size);
}
//printing the stack sent to the function.
void stack_print(pstack_t stack) {
    if (stack == NULL || stack_is_empty(stack)) {
        return ;
    }
    for (int i = stack->size - 1; i >= 0; i--) {
        stack->elem_print(stack->stack_elements[i]);
    }
}