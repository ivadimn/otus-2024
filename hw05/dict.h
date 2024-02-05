#ifndef _DICT_H_
#define _DICT_H_

#define CELL_CMP_EQ(a, b)  (strcmp((char*)(a), (char*)(b)) == 0)
typedef unsigned char alpha;

typedef enum {CHAR, INT, LONG, FLOAT, DOUBLE, STR } val_type_t;

typedef struct cell_t {
    alpha* key;
    void* value;
} cell_t;

typedef struct dict_t {
    cell_t **data;          //данные
    size_t count;           //сколько уже заполнено
    size_t size;            //всего элементов массива
    size_t limit;           //если элементов больше чем лимит пересоздаём dict
    float factor;           //степень заполнения массива по
                            //достижении которого пересоздётся dict
    float mult;             //во столько раз увеличится размер dict
    val_type_t val_type;    // тип значения ключа
} dict_t;


dict_t* create_dict(size_t, float, float, val_type_t);
void destroy_dict(dict_t* dict);

dict_t* recreate_dict(dict_t ** , cell_t*);
void put(dict_t**, alpha*, void*);
void* get(dict_t *dict, alpha* word);
void print_dict_st(dict_t*);
void print_dict(dict_t* );

unsigned long long hash_code(alpha* word);

#endif
