#ifndef TYPES_H
#define TYPES_H

typedef int8_t   int8;
typedef int16_t  int16;
typedef int32_t  int32;
typedef int64_t  int64;
typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef uint32_t uint;
typedef uint32_t bool;

typedef unsigned char byte;

typedef uintptr_t uintptr;
typedef off_t offset;
typedef size_t size;

#define false (bool)0
#define true  (bool)1

// Some macros mentioned in: https://nullprogram.com/blog/2023/10/08/
// These are really handy. :)
#if defined(__GNUC__) || defined(__clang__)
    #define assert(c) while (!(c)) __builtin_unreachable()
#else
    #define assert(c)
#endif

#define countof(a)  (size)(sizeof(a) / sizeof(*(a)))
#define lengthof(s) (countof(s) - 1)

#endif // TYPES_H
