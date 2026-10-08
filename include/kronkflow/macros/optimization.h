/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Prophecy optimizations (attributes)
*/
#ifndef PROPHECY_MACROS_OPTIMIZATION_H
    #define PROPHECY_MACROS_OPTIMIZATION_H

    // NOTE: __has_attribute(x) must not be reached when the compiler does not know it (MSVC):
    //       even behind a defined(__has_attribute) &&, it would not parse
    #if defined(__has_attribute)
        #define KF_HAS_ATTRIBUTE(x) __has_attribute(x)
    #else
        #define KF_HAS_ATTRIBUTE(x) 0
    #endif

    #if defined(__GNUC__) && (__GNUC__ >= 4)
        #define KF_GNUC 1
    #else
        #define KF_GNUC 0
    #endif

    // NOTE: On Windows, the DLL exports everything (WINDOWS_EXPORT_ALL_SYMBOLS),
    //       and a static library needs nothing
    #if defined(_WIN32) || defined(__CYGWIN__)
        #define KF_API
    #elif KF_GNUC || KF_HAS_ATTRIBUTE(visibility)
        #define KF_API __attribute__((visibility("default")))
    #else
        #define KF_API
    #endif

    #if KF_GNUC || KF_HAS_ATTRIBUTE(unused)
        #define KF_UNUSED __attribute__((unused))
    #else
        #define KF_UNUSED
    #endif

    #if KF_GNUC || KF_HAS_ATTRIBUTE(hot)
        #define KF_HOT __attribute__((hot))
    #else
        #define KF_HOT
    #endif

    #if KF_GNUC || KF_HAS_ATTRIBUTE(cold)
        #define KF_COLD __attribute__((cold))
    #else
        #define KF_COLD
    #endif

#endif /* PROPHECY_MACROS_OPTIMIZATION_H */
