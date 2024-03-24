#ifndef _TV_H_
#define _TV_H_

#include <gtk/gtk.h>

enum
{
   NAME_COLUMN,
   SIZE_COLUMN,
   DATE_COLUMN,
   DATA_COLUMN,
   N_COLUMNS
};

void activate (GtkApplication* app, gpointer user_data);

#endif
