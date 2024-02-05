/*
    Реализация функций работы со словарём
    Словарь dict принимает в качестве ключа строковое значение
    в качестве значения принимается значение в зависимости от val_type_t
*/

#include "common.h"
#include "dict.h"
#include <stdint.h>
#include <ctype.h>
#include <locale.h>

static const float DEFAULT_FACTOR = 0.72f;
static const size_t DEFAULT_SIZE = 256;
static const float MULT = 2.0f;

static cell_t* _create_cell(alpha* key, void* val, val_type_t val_type);
static void _destroy_cell(cell_t** cell);
static void _put(dict_t** _dict, cell_t *cell);
static cell_t* _get(dict_t* _dict, alpha* key);
static void _set_cell_value(cell_t* cell, void* val, val_type_t val_type);

/*
    Процедура установки значения ключа
*/
static void _set_cell_value(cell_t* cell, void* val, val_type_t val_type) {

    switch (val_type)
    {
        case CHAR:
             *((char*)cell->value) = *((char*) val);
            break;
        case INT:
            *((int*)cell->value) = *((int*) val);
            break;
        case LONG:
            *((long*)cell->value) = *((long*) val);
            break;
        case FLOAT:
            *((float*)cell->value) = *((float*) val);
            break;
        case DOUBLE:
            *((double*)cell->value) = *((double*) val);
            break;
        case STR:
            if(cell->value)
                free(cell->value);
            size_t len = strlen((char*) val);
            alpha* buf = (alpha*) alloc(len + 1);
            memcpy(buf, val, len);
            buf[len] = '\0';
            cell->value = (void*) buf;
            break;
        default:
            err_sys("Не установленный тип значения.");
            break;
    }
}
/*
    функция создания записи словаря:
    cell->key:    ключ словаря
    cell->vvalue: значение ключа
*/

static cell_t* _create_cell(alpha* key, void* val, val_type_t val_type) {
    size_t len = strlen((char*) key);
    cell_t* cell = (cell_t*) alloc(sizeof(cell_t));
    cell->key = (alpha*) alloc(len + 1);
    memcpy(cell->key, key, len);
    cell->key[len] = '\0';
    switch (val_type)
    {
        case CHAR:
            cell->value = (char*) alloc(sizeof(char));
            break;
        case INT:
            cell->value = (int*) alloc(sizeof(int));
            break;
        case LONG:
            cell->value = (long*) alloc(sizeof(long));
            break;
        case FLOAT:
            cell->value = (float*) alloc(sizeof(float));
            break;
        case DOUBLE:
            cell->value = (double*) alloc(sizeof(double));
            break;
        case STR:
            break;
        default:
            err_sys("Не установленный тип значения.");
            break;
    }
    _set_cell_value(cell, val, val_type);
    return cell;
}

//удаление ячейки словаря
static void _destroy_cell(cell_t** cell){
    free((*cell)->key);
    free((*cell)->value);
    free((*cell));
}

/*
    Процедура созхдания словаря

    size_t size            всего элементов массива
    size_t limit;          если элементов больше чем лимит пересоздаём dict
    float factor;          степень заполнения массива по достижении которого пересоздётся dict
    float mult;             во столько раз увеличится размер dict
    val_type_t val_type;    тип значения ключа
*/

dict_t* create_dict(size_t size, float factor, float mult, val_type_t val_type) {

    dict_t *dict = (dict_t*) alloc(sizeof(dict_t));

    dict->size = (size >= DEFAULT_SIZE) ? size : DEFAULT_SIZE;
    dict->data = (cell_t**) calloc(dict->size, sizeof(cell_t*));
    if (dict->data == NULL)
         err_sys("Ошибка распределения памяти: ");

    dict->count = 0;
    dict->val_type = val_type;
    dict->factor = (factor >= DEFAULT_FACTOR && factor < 1.0f )
                    ? factor : DEFAULT_FACTOR;
    dict->limit = (size_t) (dict->factor * dict->size);
    dict->mult = (mult >= MULT) ? mult : MULT;

    return dict;
}

// удаление словаря

void destroy_dict(dict_t* dict) {
    for (size_t i = 0; i < dict->size; i++)  {
        if (dict->data[i]) {
            _destroy_cell(&(dict->data[i]));
        }
    }
    free(dict->data);
    free(dict);
}

/*
    Процедура пересоздания словаря
*/
dict_t* recreate_dict(dict_t ** _dict, cell_t* cell) {
    dict_t *dict = (*_dict);
    dict_t *new_dict = create_dict((size_t)dict->size * dict->mult,
                                dict->factor, dict->mult, dict->val_type);
    size_t size = dict->size;
    for(size_t i = 0; i < size; i++) {
        if(dict->data[i] != NULL) {
            _put(&new_dict, dict->data[i]);
        }
    }
    free(dict->data);
    free(*_dict);

    _put(&new_dict, cell);
    *_dict = new_dict;
    return new_dict;
}

/*
     Положить элемент в словарь
     Вызывается если элемента с данным ключем в словаре нет
*/

static void _put(dict_t** _dict, cell_t *cell) {
    dict_t *dict = *_dict;
    unsigned long long hash = hash_code(cell->key);
    size_t index = hash % dict->size;
    if (dict->count < dict->limit) {
        if (dict->data[index] == NULL) {
            dict->data[index] = cell;
        }
        else {
            while (dict->data[index] != NULL)  {
                index++;
                if (index >= dict->size)   {
                    index = 0;
                }
            }
            dict->data[index] = cell;
        }
    }
    else {
        *_dict = recreate_dict(_dict, cell);
    }
    (*_dict)->count++;
}

/*
    Положить ключ и значение в словарь.
*/

void put(dict_t **_dict, alpha* key, void* value) {
    dict_t *dict = *_dict;
    cell_t *cell = _get(dict, key);
    if (cell) {
        _set_cell_value(cell, value, dict->val_type);
    }
    else {
        cell = _create_cell(key, value, dict->val_type);
        _put(_dict, cell);
    }
}

/*
    найти элемент словаря по ключу
*/

static cell_t* _get(dict_t* dict, alpha* key) {

    unsigned long long hash = hash_code(key);
    size_t index = hash % dict->size;
    cell_t* result = NULL;
    if (dict->data[index] != NULL)  {
        if (CELL_CMP_EQ(dict->data[index]->key, key)) {
            result = dict->data[index];
        }
        else {
            for(size_t idx = 0; idx < dict->size; idx++) {
                if (dict->data[idx] != NULL && CELL_CMP_EQ(dict->data[idx]->key, key)) {
                    result = dict->data[idx];
                    break;
                }
            }
        }
    }
    return result;
}

/*
    найти значение по ключу
*/
void* get(dict_t *dict, alpha* key) {

    cell_t * cell = _get(dict, key);
    void* result = NULL;
    if(cell)
        result = cell->value;
    return result;
}

/*
    процедура вычисления хэш-кода
*/

unsigned long long hash_code(alpha* word) {
    unsigned long long hash = 5381;
    int ch;
    while ((ch = *word++)) {
        hash = ((hash << 5) + hash) ^ ch;
    }
    return hash;
}

/*
    печать словаря
*/

void print_dict(dict_t* dict) {

    for (size_t i = 0; i < dict->size; i++) {
        if (dict->data[i])  {

            printf("Ключ: %s - значение: ", dict->data[i]->key);
            switch (dict->val_type)
            {
                case CHAR:
                    printf("%c\n", *((char*)dict->data[i]->value));
                    break;
                case INT:
                    printf("%d\n", *((int*)dict->data[i]->value));
                    break;
                case LONG:
                    printf("%ld\n", *((long*)dict->data[i]->value));
                    break;
                case FLOAT:
                    printf("%f\n", *((float*)dict->data[i]->value));
                    break;
                case DOUBLE:
                    printf("%f\n", *((double*)dict->data[i]->value));
                    break;
                case STR:
                    printf("%s\n", (alpha*)dict->data[i]->value);
                    break;
                default:
                    printf("\n");
                    break;
            }
        }
    }
}

void print_dict_st(dict_t* dict) {
    printf("\nФактор загрузки: %f\n", dict->factor);
    printf("На сколько умножаем: %f\n", dict->mult);
    printf("Размер словаря: %lu\n", dict->size);
    printf("Лимит словаря: %lu\n", dict->limit);
    printf("Количество элементов: %lu\n\n", dict->count);
}
