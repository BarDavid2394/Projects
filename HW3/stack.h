#ifndef __STACK__
#define __STACK__

#include <stddef.h>  /* size_t */
#include <stdbool.h> /* bool   */
#include <stdlib.h>
#include <stdio.h>

typedef enum {
    SUCCESS = 0, FAIL
} Result;

typedef void *elem_t;
/* ------------------------------------------ */
//Pointer to clone function which return elem_t type
typedef elem_t (*clone_t)(elem_t e);
//Pointer to print function
typedef void (*print_t)(elem_t e);
//Pointer to destroy function
typedef void (*destroy_t)(elem_t e);
//creating a stack type structure
typedef struct stack {
    elem_t *stack_elements;
    int size;
    int max_size;
    clone_t elem_clone;
    destroy_t elem_destroy;
    print_t elem_print;
} *pstack_t;

/* ------------------------------------------ */
pstack_t stack_create(size_t max_num_of_elem,
                      clone_t elem_clone,
                      destroy_t elem_destroy,
                      print_t elem_print);

/**
 * @fn void stack_destroy(pstack_t s);
 * @brief destroys the user_stack. free all the memory of the elements in the
 * stack. user_stack must be a stack created by StackCreate otherwise
 * undefined behavior)
 * @param stack - pointer to the stack to destroy
 * @return none
 */
Result stack_destroy(pstack_t stack);
//Declaration of stack's push function which returns Result type
Result stack_push(pstack_t stack, elem_t new);
//Declaration of function which checks if the stack is empty
bool stack_is_empty(pstack_t stack);
//Declaration of stack's pop function
void stack_pop(pstack_t stack);
//Declaration of function which returns the last object
elem_t stack_peek(pstack_t stack);
//Declaration of function which returns the size of the stack
size_t stack_size(pstack_t stack);
//Declaration of function which returns the capacity of the stack
int stack_capacity(pstack_t stack);
//Declaration of function which print the stack struct
void stack_print(pstack_t stack);

#endif /* __STACK__ */