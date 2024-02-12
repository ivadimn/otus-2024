#include "json_func.h"
#include <stdio.h>

w_info_t w_area = {.key = "areaName", .value = "value",
          .next = NULL, .format = "Погода в: %s\n"} ;
w_info_t w_descr = {.key = "weatherDesc", .value = "value",
          .next = NULL, .format = "Погода - описание: %s\n"} ;
w_info_t w_descr_ru = {.key = "lang_ru", .value = "value",
          .next = NULL, .format = "Погода - описание (рус.): %s\n"} ;

w_info_t winfo[] =  {
      {.key = "nearest_area", .value = NULL, .next = &w_area, .format = NULL},
      {.key = "current_condition", .value = NULL, .next = &w_descr, .format = NULL},
      {.key = "current_condition", .value = NULL, .next = &w_descr_ru, .format = NULL},
      {.key = "current_condition", .value = "temp_C", 
               .next = NULL, .format = "Температура: %s гр. Цельсия\n"},
      {.key = "current_condition", .value = "winddir16Point", 
               .next = NULL, .format = "Направление ветра: %s\n"},
      {.key = "current_condition", .value = "windspeedKmph",
               .next = NULL, .format = "Скорость ветра  %s км. ч.\n"},
      {.key = "weather", .value = "maxtempC",  
               .next = NULL, .format = "Максимальная температура: %s гр. Цельсия\n"},
      {.key = "weather", .value = "mintempC", 
               .next = NULL, .format = "Минимальная температура: %s гр. Цельсия\n"},
      };

void get_info(json_object *root) {
   size_t count = 8, first = 0;
   json_object *up = NULL, *elem = NULL;
   for (size_t i = 0; i < count; i++) {
      up = json_object_object_get(root, winfo[i].key);
      if (!up)
         continue;

      if (winfo[i].next != NULL) {
         w_info_t *next = winfo[i].next;
         elem = json_object_array_get_idx(up, first);
         up = json_object_object_get(elem, next->key);
         if (!up)
            continue;
         elem = json_object_array_get_idx(up, first);
         printf(next->format, json_object_get_string(json_object_object_get(elem, next->value)));
      }
      else {
         elem = json_object_array_get_idx(up, first);
         printf(winfo[i].format, json_object_get_string(json_object_object_get(elem, winfo[i].value)));
      }
   }
}
