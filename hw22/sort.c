#include "sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>

void sort_ins(unsigned char** keys, long* values, size_t left, size_t right) {
    size_t i, j;
    long tv;
    unsigned char* tk;
    for (i = right; i > left; i--)   
        if(less(values[i], values[i - 1])) 
            exch(keys[i], keys[i - 1], values[i], values[i - 1], long) 
    for (i = left + 2; i <= right; i++) {
        j = i;
        tk = keys[i];
        tv = values[i];
        while (less(tv, values[j - 1]))  {
            values[j] = values[j - 1];
            keys[j] = keys[j - 1];
            j--;
        }
        keys[j] = tk;
        values[j] = tv;
    }
    
}

void sort_sel(unsigned char** keys, long* values, size_t left, size_t right) {
    size_t i, j, min;
    for (i = left; i < right; i++) {   
        min = i;
        for (j = i + 1; j <= right; j++) 
            if(less(values[j], values[min])) 
                min = j;
        exch(keys[i], keys[min], values[i], values[min], long)        
        
    }
}    

void sort_shell(unsigned char** keys, unsigned long * values, size_t left, size_t right) {
    size_t i, j, h;
    unsigned long tv;
    unsigned char* tk;
    for (h = 1; h <= (right - 1) / 9; h = 3 * h + 1) ;
    for (; h > 0; h /= 3)
        for(i = left + h; i <= right; i++) {
            j = i;
            tk = keys[i];
            tv = values[i];
            while(j >= left + h && less(tv, values[j - h])) {
                keys[j] = keys[j - h];
                values[j] = values[j - h];
                j -= h;
            }
            values[j] = tv;
            keys[j] = tk;
        }
}

