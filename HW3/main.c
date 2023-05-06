#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "student.h"

#define NUM_OF_STACK    3
#define MAX_LINE_SIZE   1000
#define MAX_ARGS        10

enum args_number {ONE_ARGS = 1, TWO_ARGS = 2, FOUR_ARGS = 4};
Result one_arg_function_check(int num_arg, char *arg, int *stack_id, char *cmd);

int main(int argc, char const *argv[])
{
    pstack_t    stack_arr[NUM_OF_STACK] = {NULL};
    student_t   student;
    Result      res;
    char        entry_line[MAX_LINE_SIZE] = "";
    char        *cmd;
	char        *args[MAX_ARGS];
    char        *delimiters = (char*)" \t\n";
    int         num_arg = 0;
    int         max_num_of_elem;
    int         stack_id;
    elem_t      tmp;
    size_t      stack_sz;
    bool        is_empty;

    while(fgets(entry_line, MAX_LINE_SIZE, stdin)) {
        if('#' == entry_line[0]) {
            continue;
        }
        cmd = strtok(entry_line, delimiters);
        if (!cmd ) {
            continue;
        }
        num_arg = 0;
        for (int i = 0; i < MAX_ARGS; i++) {
            args[i] = strtok(NULL, delimiters);
            if (args[i] != NULL) {
                num_arg++;
            }
        }

        if (!strcmp(cmd, "Create_stack")) {
            if (TWO_ARGS != num_arg) {
                printf("wrong number of arguments for command %s\n", cmd);
                continue;
            }
            max_num_of_elem = atoi(args[0]);
            if (max_num_of_elem < 1) {
                printf("max_num_of_elem should be bigger then 0\n");
                continue;
            }
            stack_id = atoi(args[1]);
            if ((stack_id < 0) || (stack_id >= NUM_OF_STACK)) {
                printf("stack id should be in range [0,%d)\n", NUM_OF_STACK);
                continue;
            }
            if (NULL != stack_arr[stack_id]) {
                continue;
            }
            stack_arr[stack_id] = (pstack_t)stack_create(max_num_of_elem,
                student_clone, student_destroy, student_print);
            if(!stack_arr[stack_id]) {
                printf("failed to create stack with id %d\n", stack_id);
                return FAIL;
            }
            printf("stack with %d elements and id %d was created\n",
                max_num_of_elem,
                stack_id);
        }

        else if (!strcmp(cmd, "Add_student")) {
            if(FOUR_ARGS != num_arg) {
                printf("wrong number of arguments for command %s\n", cmd);
                continue;
            }
            student.id = atoi(args[0]);
            student.name = args[1];
            student.age = atoi(args[2]);
            stack_id = atoi(args[3]);
            if ((stack_id < 0) || (stack_id >= NUM_OF_STACK)) {
                printf("stack id should be in range [0,%d)\n", NUM_OF_STACK);
                continue;
            }
            res = stack_push(stack_arr[stack_id], ((elem_t*)&student));
            if(FAIL == res) {
                printf("failed to add student to stack with id %d\n", stack_id);
                continue;
            }
            printf("Added student to stack with id %d\n", stack_id);
        }

        else if (!strcmp(cmd, "Remove_student")) {
            res = one_arg_function_check(num_arg, args[0], &stack_id, cmd);
            if (FAIL == res) {
                continue;
            }
            stack_pop(stack_arr[stack_id]);
            printf("Removed student from stack with id %d\n", stack_id);

        }

        else if (!strcmp(cmd, "Peek_stack")) {
            res = one_arg_function_check(num_arg, args[0], &stack_id, cmd);
            if (FAIL == res) {
                continue;
            }
            tmp = stack_peek(stack_arr[stack_id]);
            student_print(tmp);
        }

        else if (!strcmp(cmd, "Stack_size")) {
            res = one_arg_function_check(num_arg, args[0], &stack_id, cmd);
            if (FAIL == res) {
                continue;
            }
            if (NULL != stack_arr[stack_id]) {
                stack_sz = stack_size(stack_arr[stack_id]);
                printf("stack %d size is: %zu\n", stack_id, stack_sz);
            }
        }

        else if (!strcmp(cmd, "Stack_is_empty")) {
            res = one_arg_function_check(num_arg, args[0], &stack_id, cmd);
            if (FAIL == res) {
                continue;
            }
            if (NULL != stack_arr[stack_id]) {
                is_empty = stack_is_empty(stack_arr[stack_id]);
                printf("stack %d is: %s\n", 
                    stack_id, 
                    is_empty ? "empty" : "not empty");
            }
        }

        else if (!strcmp(cmd, "Stack_capacity")) {
            res = one_arg_function_check(num_arg, args[0], &stack_id, cmd);
            if (FAIL == res) {
                continue;
            }
            if (NULL != stack_arr[stack_id]) {
                stack_sz = stack_capacity(stack_arr[stack_id]);
                printf("stack %d capacity is: %zu\n", stack_id, stack_sz);
            }
        }

        else if (!strcmp(cmd, "Print_stack")) {
            res = one_arg_function_check(num_arg, args[0], &stack_id, cmd);
            if (FAIL == res) {
                continue;
            }
            stack_print(stack_arr[stack_id]);
        }

        else {
            printf("illegal command\n");
        }
    }

    for(int i = 0; i < NUM_OF_STACK; i++) {
        if(stack_arr[i]) {
            stack_destroy(stack_arr[i]);
        }
    }

    return 0;
}

Result one_arg_function_check(int num_arg, char *arg, int *stack_id, char *cmd){
    if(ONE_ARGS != num_arg) {
        printf("wrong number of arguments for command %s\n", cmd);
        return FAIL;
    }
    *stack_id = atoi(arg);
    if ((*stack_id < 0) || (*stack_id >= NUM_OF_STACK)) {
        printf("stack id should be in range [0,%d)\n", NUM_OF_STACK);
        return FAIL;
    }
    return SUCCESS;
}

