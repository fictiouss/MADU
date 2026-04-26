#ifndef DEF_H
#define DEF_H
#define NULL ((void*) 0)
#define offsetof(TYPE, MEMBER) __builtin_offsetof(TYPE, MEMBER)
typedef __UINT64_TYPE__ size_t;
typedef __INT64_TYPE__  ptrdiff_t;
#endif