/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Prophecy types
*/
#ifndef PROPHECY_MACROS_TYPES_H
    #define PROPHECY_MACROS_TYPES_H
    #include <stdbool.h>
    #include <stddef.h>
    #include <stdint.h>

    typedef int kfBool;
    #define kfTrue  1
    #define kfFalse 0

    typedef kfBool (*kfHandler)(void *, void *);
    typedef void (*kfClearer)(void *);
    typedef uint64_t kfTick;
    typedef size_t   kfTaskID;

#endif /* PROPHECY_MACROS_TYPES_H */
