#ifndef _SORT_H_
#define _SORT_H_

#include <stddef.h>

#define less(a, b) ((a) < (b))
#define exch(ka, kb, va,  vb, type) { \
    unsigned char* tk = ka; ka = kb; kb = tk; \
    type tv = va; va = vb; vb = tv; }

void sort_ins(unsigned char** keys, long* values, size_t left, size_t right); 
void sort_sel(unsigned char** keys, long* values, size_t left, size_t right);
void sort_shell(unsigned char** keys, unsigned long* values, size_t left, size_t right);
#endif