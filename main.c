#include <stdio.h>

// image_preprocessing
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "image_preprocessing/operations.h"
#include "image_preprocessing/grayscale.h"
#include "image_preprocessing/rotate.h"
#include "image_preprocessing/noise_cancel.h"
#include "image_preprocessing/Gauss.h"
#include "image_preprocessing/Canny.h"
#include "image_preprocessing/Sobel.h"
#include "image_preprocessing/houghv3.h"
#include "image_preprocessing/stack.h"
#include "image_preprocessing/hough_transform.h"
#include "string.h"

// interface
#include "gtk/gtk.h"
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
    gtk_widget_set_sensitive(GTK_WIDGET(interface->preprocess_button), TRUE);

    gchar* file = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(button));
    int width = 500;
    int height = 500;
    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file_at_scale(file, width, height, TRUE, NULL);
    gtk_image_set_from_pixbuf(interface->image, pixbuf);
    //gtk_widget_hide(GTK_WIDGET(interface->load_button));

    g_free(file);
}


void on_preprocess_clicked(GtkButton* button, gpointer user_data)
{
    gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE);
    Interface* interface = user_data;
    gchar* file = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(interface->load_button));
    Gamma(file);
    Contrast("gamma.bmp");
    Gauss("contrast.bmp");
    SDL_Surface* image = display_bmp("Gauss.bmp");

        double **M = calloc(image->w, sizeof(double)); // initialisation of the Matrix

    for (int i = 0; i < image->w; i++)
    {
        M[i] = calloc(image->h, sizeof(double));
    } // end of init
    //noise removing 
    otsu_treshold("Gauss.bmp");
    Sobel("blackwhite.bmp", M);
    for (int k = 0; k < image->w; k++) // free the Matrix
    {
        free(M[k]);
    }
    free(M);

    int len = (int) strlen(file);
    char* files = file;
    if(files[len-6] == '5')
    {
        image_rotation("Sobel.bmp",35);
    }
    else
    {   
        image_rotation("Sobel.bmp",0);
    }

    otsu_treshold("rotated.bmp");
    edge_detection("blackwhite.bmp");

    int width = 500 ;
    int height = 500;
    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file_at_scale("muchachos.bmp", width, height, TRUE, NULL);
    gtk_image_set_from_pixbuf(interface->image, pixbuf);
    //gtk_widget_hide(GTK_WIDGET(interface->load_button));
}




int main(int argc, char** argv)
{
    if (argc != 2 && argc != 4)
    {
        return 1;   
    }
    

    if (strcmp(argv[1], "image_preprocessing") == 0)
    {
        printf("here");
        Gamma(argv[2]);
        Contrast("gamma.bmp");
        Gauss("contrast.bmp");
        SDL_Surface* image = display_bmp("Gauss.bmp");

        double **M = calloc(image->w, sizeof(double)); // initialisation of the Matrix

        for (int i = 0; i < image->w; i++)
        {
            M[i] = calloc(image->h, sizeof(double));
        } // end of init
        //noise removing 
        otsu_treshold("Gauss.bmp");
        Sobel("blackwhite.bmp", M);
                
                
        for (int k = 0; k < image->w; k++) // free the Matrix
        {
            free(M[k]);
        }
        free(M);

                
                
        if(strcmp(argv[3], "r") == 0)
        {
            image_rotation("Sobel.bmp",35);
        }
        else
        {
            image_rotation("Sobel.bmp",0);
        }
        otsu_treshold("rotated.bmp");
            
        edge_detection("blackwhite.bmp");
    }
    else if (strcmp(argv[1], "gtk") == 0)
    {
        gtk_init(&argc, &argv);

        GtkBuilder* builder = gtk_builder_new ();

        GError* error = NULL;
        if (gtk_builder_add_from_file(builder, "gtk/interface.glade", &error) == 0)
        {
            g_printerr("Error loading file: %s\n", error->message);
            g_clear_error(&error);
            return 1;
        }

        g_object_set(gtk_settings_get_default(),"gtk-application-prefer-dark-theme", TRUE,NULL);

        GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
        GtkButton* preprocess_button = GTK_BUTTON(gtk_builder_get_object(builder, "LaunchProcessButton"));
        GtkFileChooserButton* load_button = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "LoadButton"));
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

        gtk_widget_show(GTK_WIDGET(window));

        gtk_main();

        return 0;
    }
    return 0;
}
