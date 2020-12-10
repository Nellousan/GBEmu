/*
** EPITECH PROJECT, 2020
** auto_malloc.h
** File description:
** 
*/

#ifndef AUTO_MALLOC_H
#define AUTO_MALLOC_H

/**************************************/

// 0 to remove free attempt warning
#define A_MALLOC_WARNING 1

/**************************************/

#define AUTO_FREE_MSG1 "(auto-malloc)Warning: Free attempt on list "
#define AUTO_FREE_MSG2 " with no allocation call beforehand.\n"
#define AUTO_FREE_NO_CALL_MSG "%s%d%s", AUTO_FREE_MSG1, list, AUTO_FREE_MSG2
#define A_MALLOC_FREE 0
#define A_LIST_AMNT 10
#define LIST_MODE 0
#define ARRAY_MODE 1

#define A_LIST(x) ((x) << 24)
#define GET_A_LIST(x) ((x) >> 24)
#define GET_SIZE(x) ((x) & 0xFFFFFF)

/**************************************/

typedef struct auto_malloc_list a_malloc_t;

typedef struct auto_malloc_list {
    void *malloc_ptr;
    a_malloc_t *next;
}a_malloc_t;

/**************************************/

void *a_malloc(unsigned int val);

/**************************************/

#endif
