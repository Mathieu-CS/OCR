#include "gtk/gtk.h"
#include <stdio.h>

typedef struct Interface
{
    GtkWindow* window;
    GtkFileChooserButton* load_button;
    GtkButton* preprocess_button;
    GtkImage* image;
}Interface;

void on_load(GtkFileChooserButton* button, gpointer user_data)
{
    Interface* interface = user_data;

    gchar* file = gtk_file_chooser_get_filename(button);
    int width = 500 ;
    int height = 500;
    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file_at_scale(file, width, height, TRUE, NULL);
    gtk_image_set_from_pixbuf(interface->image, pixbuf);
    gtk_widget_hide(GTK_WIDGET(interface->load_button));

    g_free(file);
}



void on_preprocess_clicked(GtkButton* button, gpointer user_data)
{
    Interface* interface = user_data; 
    gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE);

    // Launch preprocessing
}

gboolean on_configure(GtkWidget* widget, GdkEvent* event, gpointer user_data)
{
    Interface* interface = user_data;

    gint width = gtk_widget_get_allocated_width(widget);
    gint height = gtk_widget_get_allocated_height(widget);
    
    
    return FALSE;
}

int main(int argc, char** argv)
{
    gtk_init(&argc, &argv);

    GtkBuilder* builder = gtk_builder_new ();

    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "interface.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
    GtkButton* preprocess_button = GTK_BUTTON(gtk_builder_get_object(builder, "LaunchProcessButton"));
    GtkFileChooser* load_button = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "LoadButton"));
    GtkImage* image = GTK_IMAGE(gtk_builder_get_object(builder, "image"));


    Interface interface = {
        .window = window,
        .preprocess_button = preprocess_button,
        .load_button = load_button,
        .image = image,
    };


    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(load_button, "selection-changed", G_CALLBACK(on_load), &interface);
    g_signal_connect(preprocess_button, "clicked", G_CALLBACK(on_preprocess_clicked), &interface);
    g_signal_connect(window, "configure_event", G_CALLBACK(on_configure), &interface);

    gtk_widget_show(GTK_WIDGET(window));

    gtk_main();

    return 0;
}