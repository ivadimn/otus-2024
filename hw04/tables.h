#ifndef _TABLES_H_
#define _TABLES_H_


#define SIZE_OFF 128

typedef enum {CP_1251, KOI_8R, ISO_8859_5} encode_t;

void to_utf8(char* infile, char* outfile, encode_t encode);
void print_usage(void);

#endif


