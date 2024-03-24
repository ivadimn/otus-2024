#include "file_info.h"
#include "tv.h"

static GtkTreeStore *store;
static GtkWidget *window = NULL;
static int clearing = 0;


static void destroy(/*GtkWidget *window, gpointer data*/) {
  file_info_t *dir = get_current_dir();
  delete_dir(dir);  
}

static void update_tree(file_info_t* dir, GtkTreeIter *parent_iter) {

  GtkTreeIter iter;
  char buff[12];
    
  gtk_window_set_title (GTK_WINDOW (window), dir->full_name);
  
  get_file_list(dir);
 
  for (size_t i = 0; i < dir->size; i++) {
    strftime(buff, 12, "%d.%m.%Y", localtime(&dir->flist[i].date));
    gtk_tree_store_append(store, &iter, parent_iter);
    gtk_tree_store_set(store, &iter,
                      NAME_COLUMN, dir->flist[i].name,
                      SIZE_COLUMN, dir->flist[i].size,
                      DATE_COLUMN, buff,
                      DATA_COLUMN, &dir->flist[i], -1);
  }
}

static void to_up(/*GtkApplication *app, gpointer data*/) {
    char parent_dir_name[MAX_PATH];

    file_info_t *dir = get_current_dir();
    get_parent_dir_name(parent_dir_name, dir->full_name);
    clearing = 1;
    gtk_tree_store_clear(store);
    delete_dir(dir);
    dir = create_dir(parent_dir_name);
    update_tree(dir, NULL);
    clearing = 0;
}

static void row_activated(GtkTreeView *treeview, GtkTreePath *path /*, 
                          GtkTreeViewColumn *column, gpointer userdata */) {

  char sel_dir[MAX_PATH];
  file_info_t *finfo;
  file_info_t *dir = NULL;
  GtkTreeIter iter;
  GtkTreeModel *model = gtk_tree_view_get_model(treeview);

  if (gtk_tree_model_get_iter(model, &iter, path)) {
    gtk_tree_model_get(model, &iter, DATA_COLUMN, &finfo, -1);
    clearing = 1;
    strcpy(sel_dir, finfo->full_name);
    gtk_tree_store_clear(store);
    dir = get_current_dir();
    delete_dir(dir);
    dir = create_dir(sel_dir);
    update_tree(dir, NULL);
    clearing = 0;
  }                          
}

static void selection_changed(GtkTreeSelection *selection /*, gpointer data*/) {
  GtkTreeIter iter;
  GtkTreeModel *model;
  file_info_t *finfo;

  if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
    gtk_tree_model_get(model, &iter, DATA_COLUMN, &finfo, -1);
    if (finfo->type == TYPE_DIR && finfo->flist == NULL && !clearing ) {
        update_tree(finfo, &iter);  
    }
  }
}

static GtkWidget* init_tree() {
  
  GtkWidget *tree;
  GtkTreeViewColumn *column;
  GtkCellRenderer *renderer;
  GtkTreeSelection *select;

  store = gtk_tree_store_new(N_COLUMNS, 
                              G_TYPE_STRING,
                              G_TYPE_ULONG,
                              G_TYPE_STRING,
                              G_TYPE_POINTER
                              );
  tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));

  
  renderer = gtk_cell_renderer_text_new();  
    
  column = gtk_tree_view_column_new_with_attributes("File name", renderer, 
                                                    "text", NAME_COLUMN,  
                                                    NULL);
  gtk_tree_view_column_set_resizable(column, TRUE);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);
  
  column = gtk_tree_view_column_new_with_attributes("Size", renderer, 
                                                    "text", SIZE_COLUMN, 
                                                    NULL);
  gtk_tree_view_column_set_resizable(column, TRUE);                                                  
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

  column = gtk_tree_view_column_new_with_attributes("Date modification", renderer, 
                                                    "text", DATE_COLUMN, 
                                                    NULL);
  gtk_tree_view_column_set_resizable(column, TRUE);                                                  
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

  column = gtk_tree_view_column_new();                                                   
  gtk_tree_view_column_set_visible(column, FALSE);                                                  
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

  select = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree));
  gtk_tree_selection_set_mode(select, GTK_SELECTION_SINGLE);
  g_signal_connect(G_OBJECT(select), "changed", 
                            G_CALLBACK (selection_changed), NULL);
  g_signal_connect(G_OBJECT(tree), "row-activated", 
                            G_CALLBACK (row_activated), NULL);
  
  return tree;

}

void activate (GtkApplication* app, gpointer user_data)   {

  GtkWidget *tree;
  GtkWidget* scrolled_window;
  GtkWidget *box;
  GtkWidget *button_box;
  GtkWidget *button;
  

  file_info_t* dir;
  
  window = gtk_application_window_new (app);
  
  gtk_window_set_title (GTK_WINDOW (window), "...");
  gtk_window_set_default_size (GTK_WINDOW (window), 600, 400);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(destroy), NULL);

  box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_window_set_child(GTK_WINDOW(window), box);

  button = gtk_button_new_with_label("Up");
  g_signal_connect(button, "clicked", G_CALLBACK(to_up), NULL);

  button_box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_box_append(GTK_BOX(button_box), button);
  gtk_box_append(GTK_BOX(box), button_box);
  
  dir = create_dir((char*) user_data);
  
  tree = init_tree();
  update_tree(dir, NULL);
  scrolled_window = gtk_scrolled_window_new ();
  
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW (scrolled_window), tree);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (scrolled_window),
                                                      GTK_POLICY_AUTOMATIC,
                                                      GTK_POLICY_AUTOMATIC);
  gtk_box_append(GTK_BOX(box), scrolled_window);
  gtk_widget_set_vexpand (scrolled_window, TRUE);
  gtk_window_present (GTK_WINDOW (window));

}
