#include <stdio.h>
#include "params.h"
#include"db.h"


int get_action() {
  int action;
  printf("\n");
  printf("1. Напечатать список параметров конфигурации.\n");
  printf("2. Добавить новый параметр конфигурации.\n");
  printf("3. Удалить параметр конфигурации.\n");
  printf("4. Изменить значение параметра конфигурации.\n");
  printf("5. Закрыть программу.\n");
  printf("\n");
  printf("Выберите номер действия и нажмите <ENTER>: ");
  scanf("%d", &action);
  scanf("%*c");
  return action;
}

int log_to_stderr = 1;



int main ()
{
  int action;
  open_db();
  while((action = get_action()) != 5) {
    switch (action) {
      case 1:
        print_params_list();
        break;
      case 2:
        add_param();
        break;
      case 3:
        remove_param();
        break;  
      case 4:
        change_param();
        break;

      default:
        break;
    }  
  }
  close_db();
  return 0;
}
