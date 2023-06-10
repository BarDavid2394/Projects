#ifndef INPUT_H
#define INPUT_H

#include "generic-field.h"

/**
 * @brief Checks whether the arguments "argc" and "argv" are valid.
 * If not, prints error messages to stderror.
 * @return 0 On success.
 */
int check_args(int argc, char **argv);


/**
 * @brief Parses "stdin" and prints to "stdout" all packets that match "field"
 */
void parse_input(GenericField &field);


#endif
