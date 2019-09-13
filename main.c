#include <stdio.h>
#include <gtk/gtk.h>
#include "error_handler.h"
#include "microcode.h"

//From here we define the first ui

//callback function for go button

void clicked (GtkButton* button, GtkEntry* entry){ 
    const char*s = gtk_entry_get_text(entry);
    printf("%s\n\n",s);
    if(!is_instruction(s)){
        error_handler("La stringa inserita non è nel formato di una istruzione dell Z64");
         //error_handler for a code that doesn't rappresent an instruction
        return;
    }
    code_t code;
    get_code(s,&code);
    if(!is_valid_code(&code))
      return;
    inst_t * in = code_to_inst(&code);
    print_inst(in);
    ret_value_t ret;
    generate_microcode(in,&ret);
    printf("file: %s\n",ret.filename);
    printf("numero microistruzioni: %d\n",ret.num_pass);
    if(ret.num_pass == 0)
      return;
    pid_t pid=fork();
    if (pid==0) { /* child process */

    //TODO: funzione int to char*
        char p = ret.num_pass + 48;
        static char *argv[4];
        argv[0] = "ui";
        argv[1] =ret.filename;
        argv[2] =&p;
        argv[3] =NULL;
        printf("%s\n",argv[1]);
        printf("%s\n",argv[2]);
        if(execvp("./ui",argv)<0)
          printf("errore\n");

        exit(127); /* only if execv fails */
    }
}

//callback function for x button
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

