/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Prophecy types
*/
#ifndef PROPHECY_MACROS_TYPES_H
    #define PROPHECY_MACROS_TYPES_H
    #include <stdbool.h>
    #include <stdint.h>

    typedef int prBool;
    #define knTrue  1
    #define knFalse 0

    typedef prBool (*prHandler)(void *, void *);
    typedef void (*prClearer)(void *);
    typedef uint64_t tick;

#endif /* PROPHECY_MACROS_TYPES_H */
