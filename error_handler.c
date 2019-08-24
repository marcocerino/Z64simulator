#include "error_handler.h"


void error_handler(char* c);{
	GtkWidget* window_error = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window_error),"Errore");
    GtkTextBuffer * buffer = gtk_text_buffer_new(NULL);
    gtk_text_buffer_set_text(buffer,c,-1);
    GtkWidget* error_text = gtk_text_view_new_with_buffer(buffer);
    gtk_container_add(GTK_CONTAINER(window_error),error_text);
    gtk_widget_show_all(window_error);
}

