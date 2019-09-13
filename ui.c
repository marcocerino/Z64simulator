#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>

int ctr = 0;

typedef struct data{
  GtkOverlay* o;
  GtkImage* im;
  GtkEntry* t;
  GtkEntry* n;
  char**filename ;
  char p;
}params_t;

void next(GtkButton* button, params_t* data){
  if(ctr >= data->p)
    ctr = 0;
  printf("%d\n",ctr);

  char o = ctr+48;
  gtk_entry_set_text(data->n,&o);

  GList *children, *iter;

  children = gtk_container_get_children(GTK_CONTAINER(data->o));
  for(iter = children; iter != NULL; iter = g_list_next(iter))
    gtk_widget_destroy(GTK_WIDGET(iter->data));
  g_list_free(children);

  GtkWidget* image1;
  image1 = gtk_image_new_from_file("microop/muop-architecture.png");
  gtk_container_add(GTK_CONTAINER(data->o),image1);

  printf("%s\n",data->filename[ctr]);
  if(data->filename[ctr][16] == 'I' && data->filename[ctr][17] == 'F'){
    printf("IF\n");
    gtk_entry_set_text(data->t,data->filename[ctr]);
    ctr ++;

    gtk_widget_show_all(GTK_WIDGET(data->o));
    gtk_widget_show_all(GTK_WIDGET(data->t));
    return;
  }

  FILE* f = fopen(data->filename[ctr],"r");
  if(f== NULL){
    printf("file non esistente %s\n",data->filename[ctr]);
    return;
  }
  printf("fopen\n");


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
  ctr++;
  gtk_widget_show_all(GTK_WIDGET(data->o));
  gtk_widget_show_all(GTK_WIDGET(data->t));
}

void delete_event(GtkWidget *widget, gpointer data){//callback function for x button
    gtk_main_quit ();
}

int show (int argc, char **argv,char** arr, char p,char* file_text,char* window_name){
  

  GtkWidget *window;
  GtkWidget * image1;
  GtkWidget* overlay;
  GtkWidget *grid;
  GtkWidget *button;
  GtkWidget *text;
  GtkWidget* number;
  GtkWidget* file_txt;

  gtk_init(&argc,&argv);

  //create the window
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), window_name);
  gtk_window_set_resizable(GTK_WINDOW(window),0);
  g_signal_connect (window, "delete_event",G_CALLBACK (delete_event), NULL);


  grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), grid);
  gtk_grid_set_row_baseline_position(GTK_GRID(grid),0,GTK_BASELINE_POSITION_CENTER);

  GtkTextBuffer * buff = gtk_text_buffer_new(NULL);
  gtk_text_buffer_set_text(buff,file_text,-1);
  file_txt = gtk_text_view_new_with_buffer(buff);
  gtk_grid_attach(GTK_GRID(grid),file_txt,0,0,1,1);


  number = gtk_entry_new();
  gtk_editable_set_editable(GTK_EDITABLE(number),FALSE);
  gtk_grid_attach(GTK_GRID(grid),number,0,1,1,1);


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
  data->filename = arr;
  data->im = GTK_IMAGE(image1);
  data->t = GTK_ENTRY(text);
  data->n = GTK_ENTRY(number);
  data->p =p;

  button = gtk_button_new_with_label ("GO!");
  g_signal_connect (button, "clicked", G_CALLBACK (next), data);
  gtk_grid_attach(GTK_GRID(grid),button,2,1,2,2);

  //shows the window
  gtk_widget_show_all(window);
  gtk_main();
  return 0;

}


int main(int argc, char ** argv){
  if(argc< 3)
    return -1;
  //TODO: funzione string to int
  char p = argv[2][0] -48;
  char* file = argv[1];
  printf("opening %s\nIt has %u pass\n",file,p);
  char ** arr = (char**)malloc(sizeof(char*)*p);
  char file_text[500];
   FILE* f = fopen(file,"r");
    char aux[p][50];
    int i;
    for(i=0;i<p; i++)
      strcpy(aux[i],"microistruzioni/");
    i=0;
    char * buf = (char*)malloc(sizeof(char)*100);
    size_t line_length;
    while(i<p && getline(&buf,&line_length,f) > 0){
      strcat(file_text,buf);
      if(buf[strlen(buf)-1] == '\n')
        buf[strlen(buf)-1] = 0;
      printf("buf: %s\n",buf);
      strcat(aux[i],buf);
      i++;
    }
    if(buf)
      free(buf);
  for(int i=0; i<p; i++){
    arr[i] = aux[i];
  }
  printf("calling show\n");

  show(argc,argv,arr,p,file_text,argv[1]);
  return 1;
}