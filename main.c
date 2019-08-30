#include <stdio.h>
#include <gtk/gtk.h>
#include "error_handler.h"
#include "microcode.h"

void clicked (GtkButton* button, GtkEntry* entry){ //calback function when the go button is clicked
    const char *s;
    s = gtk_entry_get_text(entry);
    printf("%s\n",s);
    if(!isInstruction(s)){
        error_handler("La stringa inserita non è nel formato di una istruzione dell Z64"); //error_handler for a code that doesn't rappresent an instruction
    }
    else{
      Code* code = getCode(s);
      if(isValidCode(code)){
        Inst * i = codeToInst(code);
        printInst(i);
        generateMicrocode(i);
        printf("end\n");
      }
    }
}

void delete_event(GtkWidget *widget, gpointer data){//callback function for x button
    gtk_main_quit ();
}


int main (int argc, char **argv){
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *button;
  GtkWidget *text;
  GtkWidget *entry;

  gtk_init(&argc,&argv);

  //create the window
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Z64 Simulator");
  gtk_window_set_resizable(GTK_WINDOW(window),0);
  g_signal_connect (window, "delete_event",G_CALLBACK (delete_event), NULL);


  //create a grid that contains the elements in the UI
  grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), grid);
  gtk_grid_set_row_baseline_position(GTK_GRID(grid),0,GTK_BASELINE_POSITION_CENTER);

  //create the header text
  GtkTextBuffer * buff = gtk_text_buffer_new(NULL);
  gtk_text_buffer_set_text(buff,"Inseire il codice dell'istruzione dello Z64",-1);
  text = gtk_text_view_new_with_buffer(buff);
  gtk_grid_attach(GTK_GRID(grid),text,0,0,1,1);


  //insert the entry element in the grid
   entry = gtk_entry_new();
   gtk_entry_set_max_length(GTK_ENTRY(entry),128);
   gtk_grid_attach(GTK_GRID(grid),entry,0,1,1,1);

  //create the go button
  button = gtk_button_new_with_label ("GO!");
  g_signal_connect (button, "clicked", G_CALLBACK (clicked), entry);
  gtk_grid_attach(GTK_GRID(grid),button,1,1,2,2);


  //shows the window
  gtk_widget_show_all(window);
  gtk_main();
  return 0;

}
