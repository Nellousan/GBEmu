/*
** EPITECH PROJECT, 2020
** auto_malloc
** File description:
** a
*/

#include <stdlib.h>
#include <stdio.h>
#include "a_malloc.h"

void *a_malloc_return(int size, a_malloc_t *a_list)
{
    a_malloc_t *a_copy = a_list;
    while (a_copy->next != NULL)
        a_copy = a_copy->next;
    a_copy->next = (a_malloc_t *) malloc(sizeof(a_malloc_t));
    a_copy->next->malloc_ptr = malloc(size);
    a_copy->next->next = NULL;
    return a_copy->next->malloc_ptr;
}

void a_free(a_malloc_t **a_list)
{
    a_malloc_t *a_copy_a = (*a_list);
    a_malloc_t *a_copy_b = (*a_list);
    while (a_copy_a->next != NULL) {
        a_copy_b = a_copy_b->next;
        free(a_copy_a->malloc_ptr);
        free(a_copy_a);
        a_copy_a = a_copy_b;
    }
    free(a_copy_a->malloc_ptr);
    free(a_copy_a);
    (*a_list) = NULL;
    return;
}

void a_free_array(a_malloc_t ***array_list)
{
    int i = 0;
    for (i = 0; i < A_LIST_AMNT; i++)
        if ((*array_list)[i] != NULL)
            break;
    if (i == A_LIST_AMNT) {
        free((*array_list));
        (*array_list) = NULL;
    }
    return;
}

void *a_malloc_list_init(int mode)
{
    a_malloc_t *a_list;
    a_malloc_t **array_list;
    if (mode == LIST_MODE) {
        a_list = (a_malloc_t *) malloc(sizeof(a_malloc_t));
        a_list->malloc_ptr = malloc(sizeof(char));
        a_list->next = NULL;
        return a_list;
    } else {
        array_list = (a_malloc_t **) malloc(sizeof(a_malloc_t *) * A_LIST_AMNT);
        for (int i = 0; i < A_LIST_AMNT; i++)
            array_list[i] = NULL;
        return array_list;
    }
}

void *a_malloc(unsigned int val)
{
    static a_malloc_t **a_list = NULL;
    int size = GET_SIZE(val);
    int list = GET_A_LIST(val);
    a_list = (a_list == NULL) ? (a_malloc_t **) a_malloc_list_init(ARRAY_MODE) : a_list;
    if (size == A_MALLOC_FREE) {
        if (a_list[list] == NULL) {
            (A_MALLOC_WARNING) ? printf(AUTO_FREE_NO_CALL_MSG) : 0;
            return NULL;
        } else {
            a_free(&a_list[list]);
            a_free_array(&a_list);
            return NULL;
        }
    } else if (a_list[list] == NULL)
        a_list[list] = (a_malloc_t *) a_malloc_list_init(LIST_MODE);
    return a_malloc_return(size, a_list[list]);
}
