#include "file_info.h"
#include "tv.h"
#include<string.h>


int main(int argc,char *argv[]){

  GtkApplication *app;
  int status;
  char cur_dir[MAX_NAME];

  app = gtk_application_new ("gtk.treeview.example", G_APPLICATION_FLAGS_NONE);

  if (argc == 2) {
    strcpy(cur_dir, argv[1]);
  }
  else {
    getcwd(cur_dir, MAX_NAME - 1);
  }
  
  g_signal_connect (app, "activate", G_CALLBACK (activate), cur_dir);
  status = g_application_run (G_APPLICATION (app), 1, argv);
  g_object_unref (app);

  return status;

}