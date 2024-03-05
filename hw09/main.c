/*
  Все имена функций как в ассемблерном коде
  Для реализации списков предложена структура list_node_t
  Добавлена функция освобождения памяти node_free.
*/

#include <stdio.h>
#include <stdlib.h>


char *int_format = "%ld ";
long data[] = {4, 8, 15, 16, 23, 42};
const int data_length = 6;

typedef void (*print_func)(long);
typedef long (*odd_func)(long);

typedef struct list_node_t {
    long value;
    struct list_node_t* next;
} list_node_t;

void node_free(list_node_t* node) {
    list_node_t* lnode = node; 
    while(node) {
        lnode = node->next;    
        free(node);
        node = lnode;
    }    
}

/*void node_free(list_node_t* node) {
    if (node) {
        node_free(node->next);
        free(node);
    }    
}*/

list_node_t* add_element(list_node_t* head, long value) {

    list_node_t *node = (list_node_t *) malloc(sizeof(list_node_t));
    if (node == NULL) {
        printf("Ошибка выделения памяти!!!");
        exit(EXIT_FAILURE);
    }
    node->value = value;
    node->next = head;
    return node;
}

void print_int(long value) {
    printf(int_format, value);
}

long p(long val) {
    return val & 1;
}

list_node_t* f(list_node_t* node, list_node_t* odd, odd_func ofunc) {
    if (node == NULL)
        return odd;
    if (ofunc(node->value)) {
        odd = add_element(odd, node->value);
    }
    return f(node->next, odd, ofunc);;
}

void m(list_node_t* node, print_func pfunc) {
    list_node_t* lnode = node;
    if (lnode == NULL)
        return;
    pfunc(lnode->value);
    m(lnode->next, pfunc);
}

int main() {

    list_node_t* head = NULL;
    list_node_t* odd = NULL;
    int index = data_length - 1;
    while(index >= 0) {
        head = add_element(head, data[index]);
        index--;
    }
    m(head, print_int);
    puts("");
    odd = f(head, odd, p);
    m(odd, print_int);
    puts("");
    node_free(head);
    node_free(odd);
    return 0;

}
