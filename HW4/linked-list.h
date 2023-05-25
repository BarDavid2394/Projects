#ifndef LINKED_LIST_H
#define LINKED_LIST_H

/* Defines the type "size_t", which is the standard for sizes in C */
#include <stddef.h>

/* User function, clones “element” to “output”. Returns 0 on success. */
typedef int(*element_clone_t)(void *element, void **output);

/* User function, destroys “element”. Always succeed */
typedef void(*element_destroy_t)(void *element);

/* Opaque data structures: both are defined elsewhere */
struct list;
struct iterator;

/**
 * @brief Creates a new list
 * @param elem_clone A user function that clones elements
 * @param elem_destroy A user function that destroys elements 
 * @returns A pointer to the list, or NULL on error
 */
struct list* list_init(element_clone_t elem_clone,
                       element_destroy_t elem_destroy); 

/**
 * @brief Free all allocated data on list (including elements)
 */
void list_destroy(struct list *list); /* Always succeed */

/**
 * @brief Returns the size "list"
 * @param list A pointer to list
 * @returns The list's number of elements
 */
size_t list_size(struct list *list);

/**
 * @brief Pushes a new node at the end of the list
 * @param list A pointer to list
 * @param element A pointer to the element.
 * @returns 0 on success
 * @note "list" holds a copy of "element" (clone)
 */
int list_push_back(struct list *list, void *element);

/**
 * @brief Pushes a new node at the beginning of the list
 * @param list A pointer to list
 * @param element A pointer to the element.
 * @returns 0 on success
 * @note "list" holds a copy of "element" (clone)
 */
int list_push_front(struct list *list, void *element);

/**
 * @brief Inserts "element" to "list", after the iterator "it" 
 * @param list A pointer to list
 * @param it A valid iterator to an element in "list".
 * @param element A pointer to the element.
 * @returns 0 on success
 * @note Fails if "it" is NULL
 * @note Assumes that "it" points to element in "list",
 * the user must validate it
 */
int list_insert(struct list *list, struct iterator *it, void *element);

/**
 * @brief Removes an element pointed by "it" in "list"
 * @param list A pointer to list
 * @param it An iterator wihin "list".
 * @returns 0 on success
 * @note Fails if "it" is NULL
 * @note Assumes that "it" points to element in "list",
 * the user must validate it
 */
int list_remove(struct list *list, struct iterator *it);

/**
 * @brief Returns an iterator to the beginning of the list
 * @param list A pointer to list
 * @returns A pointer to an iterator object
 * @note Returns NULL if the list is empty or on error
 */
struct iterator* list_begin(struct list *list); 

/**
 * @brief Returns an iterator to the end of the list
 * @param list A pointer to list
 * @returns A pointer to an iterator object
 * @note Returns NULL if the list is empty or on error
 */
struct iterator* list_end(struct list *list);

/**
 * @brief Returns an iterator to the element after "it"
 * @param it Am iterator
 * @note Returns NULL if "it" is NULL or if there is no next element
 */
struct iterator* list_next(struct iterator *it);

/**
 * @brief Returns an iterator to the element before "it"
 * @param it Am iterator
 * @note Returns NULL if "it" is NULL or if there is no previous element
 */
struct iterator* list_prev(struct iterator *it);

/**
 * @brief Returns a pointer to the element pointed by iterator "it" or NULL
 * on error.
 * @param "it" An iterator
 */
void* list_get(struct iterator *it); 

#endif
