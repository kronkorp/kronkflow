/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Prophecy optimizations (attributes)
*/
#ifndef PROPHECY_MACROS_OPTIMIZATION_H
    #define PROPHECY_MACROS_OPTIMIZATION_H

    #if defined(__GNUC__) && (__GNUC__ >= 4) || defined(__has_attribute) && __has_attribute(visibility)
        #define KF_API __attribute__((visibility("default")))
    #else
        #define KF_API
    #endif

    #if defined(__GNUC__) && (__GNUC__ >= 4) || defined(__has_attribute) && __has_attribute(unused)
        #define KF_UNUSED __attribute__((unused))
    #else
        #define KF_UNUSED
    #endif

    #if defined(__GNUC__) && (__GNUC__ >= 4) || defined(__has_attribute) && __has_attribute(hot)
        #define KF_HOT __attribute__((hot))
    #else
        #define KF_HOT
    #endif

    #if defined(__GNUC__) && (__GNUC__ >= 4) || defined(__has_attribute) && __has_attribute(cold)
        #define KF_COLD __attribute__((cold))
    #else
        #define KF_COLD
    #endif

#endif /* PROPHECY_MACROS_OPTIMIZATION_H */
