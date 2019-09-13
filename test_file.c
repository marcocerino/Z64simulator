#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>

typedef struct data{
  GtkOverlay* o;
  GtkImage* im;
  GtkEntry* t;
  char*filename ;
}params_t;

void next(GtkButton* button, params_t* data){
  FILE* f = fopen(data->filename,"r");
  if(f== NULL){
    printf("file non esistente %s\n",data->filename);
    return;
  }
  printf("fopen\n");

  GList *children, *iter;

  children = gtk_container_get_children(GTK_CONTAINER(data->o));
  for(iter = children; iter != NULL; iter = g_list_next(iter))
    gtk_widget_destroy(GTK_WIDGET(iter->data));
  g_list_free(children);

  GtkWidget* image1;
  image1 = gtk_image_new_from_file("microop/muop-architecture.png");
  gtk_container_add(GTK_CONTAINER(data->o),image1);

  printf("%s\n",data->filename);
  GtkWidget* i;
  char * buf = (char*)malloc(sizeof(char)*100);
  size_t line_length;
  int k = 0;
  while(getline(&buf,&line_length,f) > 0){
    printf("buf: %s\n",buf);
    if(buf[strlen(buf)-1]== '\n')
        buf[strlen(buf)-1] = 0;
      if(k==0){
          gtk_entry_set_text(data->t,buf);
      }
       else{
        i = gtk_image_new_from_file(buf);
        gtk_overlay_add_overlay(data->o,i);
      }
      k++;
    printf("%s\n",buf);
  }
  if(buf)
    free(buf);
  fclose(f);
  gtk_widget_show_all(GTK_WIDGET(data->o));
  gtk_widget_show_all(GTK_WIDGET(data->t));
}

void delete_event(GtkWidget *widget, gpointer data){//callback function for x button
    gtk_main_quit ();
}

int show (int argc, char **argv,char* filename){
  

  GtkWidget *window;
  GtkWidget * image1;
  GtkWidget* overlay;
  GtkWidget *grid;
  GtkWidget *button;
  GtkWidget *text;

  gtk_init(&argc,&argv);

  //create the window
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Z64 Simulator");
  gtk_window_set_resizable(GTK_WINDOW(window),0);
  g_signal_connect (window, "delete_event",G_CALLBACK (delete_event), NULL);


  grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), grid);
  gtk_grid_set_row_baseline_position(GTK_GRID(grid),0,GTK_BASELINE_POSITION_CENTER);


  text = gtk_entry_new();
  gtk_editable_set_editable(GTK_EDITABLE(text),FALSE);
  gtk_grid_attach(GTK_GRID(grid),text,1,1,1,1);

  //create the overlay
  overlay = gtk_overlay_new();
  //create base image
  image1 = gtk_image_new_from_file("microop/muop-architecture.png");
  gtk_container_add(GTK_CONTAINER(overlay),image1);
  gtk_grid_attach(GTK_GRID(grid),overlay,1,0,1,1);
  
  
  params_t* data;
  data->o = GTK_OVERLAY(overlay);
  data->filename = filename;
  data->im = GTK_IMAGE(image1);
  data->t = GTK_ENTRY(text);

  button = gtk_button_new_with_label ("GO!");
  g_signal_connect (button, "clicked", G_CALLBACK (next), data);
  gtk_grid_attach(GTK_GRID(grid),button,2,1,2,2);

  //shows the window
  gtk_widget_show_all(window);
  gtk_main();
  return 0;

}


int main(int argc, char ** argv){
  if(argc< 2)
    return -1;
  
  char* arr = (char*)malloc(sizeof(char)*50);
  strcpy(arr,argv[1]);
  show(argc,argv,arr);
  return 1;
}