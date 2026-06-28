/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Prophecy optimizations (attributes)
*/
#ifndef PROPHECY_MACROS_OPTIMIZATION_H
    #define PROPHECY_MACROS_OPTIMIZATION_H

    #if defined(__GNUC__) && (__GNUC__ >= 4) || defined(__has_attribute) && __has_attribute(visibility)
        #define PR_API __attribute__((visibility("default")))
    #else
        #define PR_API
    #endif

    #if defined(__GNUC__) && (__GNUC__ >= 4) || defined(__has_attribute) && __has_attribute(unused)
        #define PR_UNUSED __attribute__((unused))
    #else
        #define PR_UNUSED
    #endif

    #if defined(__GNUC__) && (__GNUC__ >= 4) || defined(__has_attribute) && __has_attribute(hot)
        #define PR_HOT __attribute__((hot))
    #else
        #define PR_HOT
    #endif

    #if defined(__GNUC__) && (__GNUC__ >= 4) || defined(__has_attribute) && __has_attribute(cold)
        #define PR_COLD __attribute__((cold))
    #else
        #define PR_COLD
    #endif

#endif /* PROPHECY_MACROS_OPTIMIZATION_H */
