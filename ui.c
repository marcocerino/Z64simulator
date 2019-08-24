#include <gtk/gtk.h>
#include <stdlib.h>
#include "useful.c"
#include <string.h>

//given the path to the images of the correct instruction, we want to open them in a UI

typedef struct data{
	GtkImage* image;
	char * path;
	int n_step;
}data;  //data structure for params passing to clicked button callback

int ctr = 0; //usefull counter;

void next(GtkButton* button, data* d){
	GtkImage* image = d->image;
	if(ctr==d->n_step)
		ctr = 0;
	else 
		ctr++;
	char* filename[100];
	strcpy(filename,data->path);
	strcat(filename,itoa(ctr));
	strcat(filename,".jpg");
	gtk_image_clear(image);
	gtk_image_set_from_file(image,filename);	
}

void ui(char* path, int n_step){
	GtkWidget *window;
	GtkWidget *grid;
  	GtkWidget *text;
  	GtkWidget * image;
  	GtkWidget* button;
  	GtkWidget* box;

  	//create the window
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Z64 Simulator");
  gtk_window_set_resizable(GTK_WINDOW(window),0);
  

  //create a grid that contains the elements in the UI
  grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), grid);
  gtk_grid_set_row_baseline_position(GTK_GRID(grid),0,GTK_BASELINE_POSITION_CENTER);

  //create the header text
  GtkTextBuffer * buff = gtk_text_buffer_new(NULL);
  gtk_text_buffer_set_text(buff,"Prova",-1);
  text = gtk_text_view_new_with_buffer(buff);


  image = gtk_image_new_from_file(strcat(path,"0.jpg"));
  gtk_grid_attach(GTK_GRID(grid),image,0,0,1,1);

  data* d = (data*)malloc(sizeof(data));
  data->image = image;
  data->path = path;
  data->n_step = n_step;

  button = gtk_button_new_with_label ("next");
  g_signal_connect (button, "clicked", G_CALLBACK (next), data);
  


  //shows the window
  gtk_widget_show_all(window);

}