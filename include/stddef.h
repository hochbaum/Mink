#ifndef MINK_STDDEF_H
#define MINK_STDDEF_H

#define NULL ((void*) 0)

/* This is some ugly preprocessor magic creating increasing _ignored<n> names.
 * And nope, there is NO prettier way. */
#define IGNORED _IGNORED_CONCAT1(_ignored, __COUNTER__);
#define _IGNORED_CONCAT1(x, y) _IGNORED_CONCAT2(x, y)
#define _IGNORED_CONCAT2(x, y) x##y

#define BIT(num, i) (num & (1UL << i))

typedef signed long size_t;

#endif //MINK_STDDEF_H
