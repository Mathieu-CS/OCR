#include "ai/neuralnetwork.h"
#include "solver/solver.h"
#include <err.h>
#include <stdio.h>
#include <math.h>

#include <sys/stat.h>
#include <fcntl.h>
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

// rect_reconstruction
#include "rect_reconstruction/reconstruction.h"



// interface
#include "gtk/gtk.h"

#define N 9 

int square(int x1, int y1, int x2, int y2)
{
    int w = x2 - x1;
    int h = y2 - y1;
    int diff = w - h;

    if (diff >= -5 && diff <= 5)
    {
        return (w + h) / 2;
    }
    else
    {
        return 0;
    }
}

// return the coordinates of the maximum square found

int __detect_rect(SDL_Surface* image,int x1,int y1)
{
    Uint32 testpixel;
    Uint8 r,g,b;

    int mw = 0;

    for (int j = image->h/2; j < image->h; j++)
    {
        for (int i = image->w/2; i < image->w; i++)
        {
            testpixel = get_pixel(image, i , j);
            SDL_GetRGB(testpixel, image->format, &r,&g,&b);

            if (r == 0 && g == 255 && b == 0) // if intersection
            {
                // square ?
                int temp = square(x1, y1, i, j);

                if (temp > 0)
                {
                    if (temp > mw) // square detected bigger than previous max square
                    {
                        mw = temp;                      
                    }
                }
            }
        }
    }

    return mw;
}

int* detect_rect(SDL_Surface* image)
{
    Uint32 currentpixel;
    Uint8 r,g,b;
    int *p = malloc(3*sizeof(int));


    int rw = 0;
    int rx = 0;
    int ry = 0;

    for (int j = 0; j < image->h/2; j++)
    {
        for (int i = 0; i < image->w/2; i++)
        {
            currentpixel = get_pixel(image, i , j);
            SDL_GetRGB(currentpixel, image->format, &r,&g,&b);

            if (r == 0 && g == 255 && b == 0)
            {
                //printf("intersection detected at %i,%i\n", i, j);
                int temp = __detect_rect(image, i, j);
                if (temp > rw)
                {
                    rx = i;
                    ry = j;
                    rw = temp;
                }
                
            }
            
        }
    }
    p[0] = rx;
    p[1] = ry;
    p[2] = rw;
    return p;
}

// -------------------------------------------------------
//                    END OF INTERSECTIONS
// -------------------------------------------------------

// -------------------------------------------------------
//                    PROCESSING
// -------------------------------------------------------

// inverts black and white pixels
void invert (SDL_Surface* image)
{
    Uint32 black = SDL_MapRGB(image->format, 0, 0, 0);
    Uint32 white = SDL_MapRGB(image->format, 255, 255, 255);

    Uint32 pixel;
    Uint8 r,g,b;

    for (int i = 0; i < image->w; i++)
    {
        for (int j = 0; j < image->h; j++)
        {
            pixel = get_pixel(image,i,j);
            SDL_GetRGB(pixel, image->format, &r,&g,&b);

            if (r > 100)
            {
                put_pixel(image,i,j,black);
            }
            else
            {
                put_pixel(image,i,j,white);
            }      
        }
    }
    
}

SDL_Surface* move(SDL_Surface* image, int decalagex, int decalagey)
{
    Uint32 pixel;
    Uint8 r1,g1,b1;
    SDL_Surface* centered = SDL_CreateRGBSurface(0,28,28,32,0,0,0,0);

    for (int i = 0; i < image->w; i++)
    {
        for (int j = 0; j < image->h; j++)
        {
            pixel = get_pixel(image,i,j);
            SDL_GetRGB(pixel, image->format, &r1,&g1,&b1);

            if (r1 == 255)
            {
                if (i - decalagex > 0 && j - decalagey && i - decalagex < 28 && j - decalagey < 28)
                {
                    put_pixel(centered, i - decalagex, j - decalagey, pixel);
                }
            }
        }
    }

    return centered;
}

// centers the number in the subimage
SDL_Surface* center(SDL_Surface* image)
{
    int totx = 0;
    int toty = 0;
    int numberpixel = 0;
    SDL_Surface* BlackSurface = SDL_CreateRGBSurface(0,28,28,32,0,0,0,0);
    
    Uint32 pixel;
    Uint8 r,g,b;

    for (int i = 0; i < image->w; i++)
    {
        for (int j = 0; j < image->h; j++)
        {
            pixel = get_pixel(image,i,j);
            SDL_GetRGB(pixel, image->format, &r,&g,&b);

            if (r == 255)
            {
                totx += i;
                toty += j;
                numberpixel++;   
            }
        }
    }

    if (numberpixel != 0)
    {
        float indicatorx = ((float)totx/numberpixel) - 14;
        float indicatory = ((float)toty/numberpixel) - 14;

        return move(image, (int) floor(indicatorx), (int) floor(indicatory));
    }
    else
    {
        return BlackSurface;
    }
}

int checkx(SDL_Surface* image, int x, int y)
{
    Uint32 pixel;
    Uint8 r, g, b;
    for (int i = x; i < image->w; i++)
    {
        pixel = get_pixel(image, i, y);
        SDL_GetRGB(pixel, image->format, &r,&g,&b);
        if (r == 0 && g == 0 && b == 0)
        {
            return 1;
        }
    }
    return 0;
}

int checky(SDL_Surface* image, int x, int y)
{
    Uint32 pixel;
    Uint8 r, g, b;
    for (int i = y; i < image->h; i++)
    {
        pixel = get_pixel(image, x, i);
        SDL_GetRGB(pixel, image->format, &r,&g,&b);
        if (r == 0 && g == 0 && b == 0)
        {
            return 1;
        }
    }
    return 0;
}

void __clean(SDL_Surface* image)
{
    int x = 0;
    int y = 0;
    Uint32 white = SDL_MapRGB(image->format, 255, 255, 255);

    while (checkx(image, x, y) == 1 && checky(image, x, y) == 1)
    {
        x++;
        y++;
    }
    
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < image->h; j++)
        {
            put_pixel(image, x, y, white);
        }
    }
    
    for (int i = 0; i < image->w; i++)
    {
        for (int j = 0; j < y; j++)
        {
            put_pixel(image, x, y, white);
        }
    }
    
}

// cleans the borders of the subimages
void clean(SDL_Surface* image)
{
    //__clean(image);
    int limitw = image->w/7;
    int limith = image->h/7;
    Uint32 white = SDL_MapRGB(image->format, 255, 255, 255);

    // up clean

    for (int i = 0; i < (limith * 7)/5; i++)
    {
        for (int j = 0; j < image->w; j++)
        {
            put_pixel(image, j, i, white);
        }
    }
    
    // down clean

    for (int i = image->h - limith; i < image->h; i++)
    {
        for (int j = 0; j < image->w; j++)
        {
            put_pixel(image, j, i, white);
        }
    }

    // left clean

    for (int i = 0; i < image->h; i++)
    {
        for (int j = 0; j < (limitw * 7)/5; j++)
        {
            put_pixel(image, j, i, white);
        }
    }
    
    // right clean

    for (int i = 0; i < image->h; i++)
    {
        for (int j = image->w - limitw; j < image->w; j++)
        {
            put_pixel(image, j, i, white);   
        }
    }
}

// splits the image in 81 subimages
void split(SDL_Surface* image, int x0, int y0, int width, int length)
{
    width = (width/9)*9;
    length = width;

    int pathx = width/9;
    int pathy = pathx;

    SDL_Rect* splitrect = malloc(sizeof(SDL_Rect));
    
    splitrect->w = pathx;
    splitrect->h = pathy;
    splitrect->x = x0;
    splitrect->y = y0;

    SDL_Surface* tosave = SDL_CreateRGBSurface(0, splitrect->w, splitrect->h, 32, 0, 0, 0, 0);

    int k = 0;
    char tot[15];

    int i = x0;
    int j = y0;

    while (j < y0 + length)
    {
        while (i < x0 + width)
        {
            splitrect->x = i;
            splitrect->y = j;
            snprintf(tot, 15, "%i.bmp", k);

            SDL_BlitSurface(image, splitrect, tosave, NULL);
            clean(tosave);
            double zoomx = ((double) 28) / tosave->w;
            double zoomy = ((double) 28) / tosave->h;

            SDL_Surface* totreat = rotozoomSurfaceXY(tosave, 0, zoomx, zoomy, 0);
            
            //SDL_SaveBMP(totreat, tot);
            
            invert(totreat);
            SDL_Surface* final = center(totreat);

            SDL_SaveBMP(final, tot);

            SDL_FreeSurface(final);
            SDL_FreeSurface(totreat);

            k++;
            i = i + pathx;
        }
        i = x0;
        j = j + pathy;
    }
    
    free(splitrect);
    SDL_FreeSurface(tosave);
}

void draw_square(SDL_Surface* image, int x, int y, int w)
{
    Uint32 blue = SDL_MapRGB(image->format, 0, 0, 255);

    for (int i = 0; i < w; i++)
    {
        put_pixel(image, x + i, y, blue);
        put_pixel(image, x + w, y + i, blue);
        put_pixel(image, x + i, y + w, blue);
        put_pixel(image, x, y + i, blue);
    }
    
    SDL_SaveBMP(image, "rect.bmp");
}

////////////////////////////////////////////////

typedef struct Interface
{
    GtkWindow* window;
    GtkFileChooserButton* load_button;
    GtkButton* preprocess_button;
    GtkButton* split_button;
    GtkButton* detect_square;
    GtkButton* feedAI_button;
    GtkButton* solve_button;
    GtkButton* start;
    GtkButton* close_button;
    GtkButton* restart;
    GtkImage* image;
    GtkImage* image1;
    GtkImage* image2;
    GtkImage* image3;
    GtkStack* stack;
}Interface;

void on_load(GtkFileChooserButton* button, gpointer user_data)
{
    Interface* interface = user_data;
    gtk_widget_set_sensitive(GTK_WIDGET(interface->preprocess_button), TRUE);

    gchar* file = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(button));
    int width = 707;
    int height = 707;
    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file_at_scale(file, width, height, TRUE, NULL);
    gtk_image_set_from_pixbuf(interface->image, pixbuf);
    //gtk_widget_hide(GTK_WIDGET(interface->load_button));

    g_free(file);
    gtk_widget_set_sensitive(GTK_WIDGET(interface->preprocess_button), TRUE);
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
    otsu_treshold("Gauss.bmp", "blackwhite.bmp");
    Sobel("blackwhite.bmp", M);
    for (int k = 0; k < image->w; k++) // free the Matrix
    {
        free(M[k]);
    }
    free(M);
    SDL_FreeSurface(image);
    otsu_treshold("Sobel.bmp", "blackwhite2.bmp");
    edge_detection("blackwhite2.bmp");

    int width = 707;
    int height = 707;
    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file_at_scale("muchachos.bmp", width, height, TRUE, NULL);
    gtk_image_set_from_pixbuf(interface->image, pixbuf);
    gtk_image_set_from_pixbuf(interface->image1, pixbuf);
    gtk_stack_set_visible_child_name(interface->stack, "page2");
    //gtk_widget_hide(GTK_WIDGET(interface->load_button));

}

void on_detect_square(GtkButton* button, gpointer user_data)
{
    gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE);
    Interface* interface = user_data;
    // argv[1] = edgedetecion.bmp
    // argv[2] = contrast.bmp

    SDL_Surface* hough = display_bmp("muchachos.bmp");
    SDL_Surface* contrast = display_bmp("blackwhite.bmp");

    int* coor = detect_rect(hough);

    /*printf("image detected\n");
    printf("x = %i\n", coor[0]);
    printf("y = %i\n", coor[1]);
    printf("width = %i\n", coor[2]);*/


    draw_square(contrast, coor[0], coor[1], coor[2]);
    split(contrast, coor[0], coor[1], coor[2], coor[2]);

    SDL_FreeSurface(hough);
    SDL_FreeSurface(contrast);
    free(coor);
    int width = 707;
    int height = 707;
    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file_at_scale("rect.bmp", width, height, TRUE, NULL);
    gtk_image_set_from_pixbuf(interface->image1, pixbuf);
    gtk_image_set_from_pixbuf(interface->image2, pixbuf);
    gtk_stack_set_visible_child_name(interface->stack, "page3");
}

void on_reconstruction(GtkButton* button, gpointer user_data)
{
    gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE);
    Interface* interface = user_data;
    reconstruction("grid_00.result", "grid_00");
    //printf("HERE\n");
    int width = 707;
    int height = 707;
    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file_at_scale("end.bmp", width, height, TRUE, NULL);
    gtk_image_set_from_pixbuf(interface->image3, pixbuf);
}

void on_feedAI(GtkButton* button, gpointer user_data)
{
    Interface* interface = user_data;
    gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE);
    FILE* fptr2;

    fptr2 = fopen("grid_00", "w");
    for (int i = 0; i < 81; i++)
    {
        //printf("%i\n", i);
        char *a;
        if (i < 10)
            a = malloc(7*sizeof(char));
        else
            a = malloc(8*sizeof(char));
        sprintf(a, "%i.bmp", i);
        int b = -1;
        if (isDigits2(a))
        {
            b = neuralNetwork(a);
        }
        if (b == -1)
            fprintf(fptr2, ".");
        else
            fprintf(fptr2, "%d", b);
        if ((i+1) % 9 == 0)
        {
            fprintf(fptr2, "\n");
            if ((i+1) % 27 == 0 && i != 80)
                fprintf(fptr2, "\n");
        }
        else if ((i+1) % 3 == 0)
        {
            fprintf(fptr2, " ");
        }
    }
    fclose(fptr2);
    gtk_widget_set_sensitive(GTK_WIDGET(interface->solve_button), TRUE);
    
}

void on_solve(GtkButton* button, gpointer user_data)
{
    (void)button;
    
    Interface* interface = user_data;
    char* param = "grid_00";
    char filename[15];
    
    FILE *fo;
    fo = fopen(param, "r");
    char input[82];
    input[0] = '\0';
    for (int i = 0; i < 27; i++)
    {
        //printf("%i\n", i);
        char temp[4];
        int a = fscanf(fo, "%s", temp);
        if (a == -99)
            printf("prob\n");
        strcat(input, temp);
    }
    //printf("%s\n", input);
    input[81] = '\0';

    fclose(fo);

    int grid[N][N];
    
    int count = 0;
    int line = 0;
    for (size_t i = 0; i < 9;)
    {
        if (input[count] != '\n' || line % 3 == 0)
        {
            for (size_t j = 0; j < 9; count++)
            { 
                if (input[count] != ' ' && input[count] != '\n')
                {
                    if (input[count] == '.')
                    {
                        grid[i][j] = 0;
                    }
                    else
                    {
                        char c = input[count];
                        grid[i][j] = c - '0';
                    }
                    j++;
                }
            }
            i++;
        }
        else
        {
            line++;
        }
    }

    
    // 0 means unassigned cells
    /*if (solveSuduko(grid, 0, 0)==1)
        print(grid);
    else
    printf("No solution exists");*/

    solveSuduko(grid, 0, 0);
    FILE *fp;
    sprintf(filename, "%s.result", param);
    fp = fopen(filename, "w");
    fprintf(fp, "%s", sudokuStr(grid));
    fclose(fp);
    gtk_stack_set_visible_child_name(interface->stack, "page4");
    gchar* file = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(interface->load_button));
    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file_at_scale(file, 707, 707, TRUE, NULL);
    gtk_image_set_from_pixbuf(interface->image3, pixbuf);
}

void on_start(GtkButton* start, gpointer user_data)
{
    (void)start;
    Interface* interface = user_data;
    gtk_stack_set_visible_child_name(interface->stack, "page1");
    gtk_widget_set_visible(GTK_WIDGET(interface->restart), TRUE);
}

void on_close(GtkButton* close, gpointer user_data)
{
    (void)close;
    Interface* interface = user_data;
    gtk_widget_destroy(GTK_WIDGET(interface->window));
}

void on_restart(GtkButton* restart, gpointer user_data)
{
    (void)restart;
    Interface* interface = user_data;
    gtk_stack_set_visible_child_name(interface->stack, "page1");
    gchar* file = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(interface->load_button));
    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file_at_scale(file, 707, 707, TRUE, NULL);
    gtk_image_set_from_pixbuf(interface->image, pixbuf);
    gtk_widget_set_sensitive(GTK_WIDGET(interface->detect_square), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(interface->feedAI_button), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(interface->solve_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(interface->split_button), TRUE);
}

int main(int argc, char** argv)
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


    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
    GtkButton* preprocess_button = GTK_BUTTON(gtk_builder_get_object(builder, "LaunchProcessButton"));
    GtkButton* split_button = GTK_BUTTON(gtk_builder_get_object(builder, "split_button"));
    GtkButton* detect_button = GTK_BUTTON(gtk_builder_get_object(builder, "detect_square"));
    GtkButton* feedAI = GTK_BUTTON(gtk_builder_get_object(builder, "feedAI"));
    GtkButton* solve_button = GTK_BUTTON(gtk_builder_get_object(builder, "solve_button"));
    GtkButton* start = GTK_BUTTON(gtk_builder_get_object(builder, "Start"));
    GtkButton* close_button = GTK_BUTTON(gtk_builder_get_object(builder, "close_button"));
    GtkButton* restart = GTK_BUTTON(gtk_builder_get_object(builder, "restart"));
    GtkFileChooserButton* load_button = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "LoadButton"));
    GtkImage* image = GTK_IMAGE(gtk_builder_get_object(builder, "image"));
    GtkImage* image1 = GTK_IMAGE(gtk_builder_get_object(builder, "image1"));
    GtkImage* image2 = GTK_IMAGE(gtk_builder_get_object(builder, "image2"));
    GtkImage* image3 = GTK_IMAGE(gtk_builder_get_object(builder, "image3"));
    GtkStack* stack = GTK_STACK(gtk_builder_get_object(builder, "Stack"));

    gtk_window_set_title(window, "Sudoku Solver");
    
    Interface interface = {
        .window = window,
        .preprocess_button = preprocess_button,
        .detect_square = detect_button,
        .split_button = split_button,
        .feedAI_button = feedAI,
        .solve_button = solve_button,
        .start = start,
        .close_button = close_button,
        .restart = restart,
        .load_button = load_button,
        .image = image,
        .image1 = image1,
        .image2 = image2,
        .image3 = image3,
        .stack = stack,
    };


    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(close_button, "clicked", G_CALLBACK(on_close), &interface);
    g_signal_connect(load_button, "selection-changed", G_CALLBACK(on_load), &interface);
    g_signal_connect(preprocess_button, "clicked", G_CALLBACK(on_preprocess_clicked), &interface);
    g_signal_connect(detect_button, "clicked", G_CALLBACK(on_detect_square), &interface);
    g_signal_connect(split_button, "clicked", G_CALLBACK(on_reconstruction), &interface);
    g_signal_connect(feedAI, "clicked", G_CALLBACK(on_feedAI), &interface);
    g_signal_connect(solve_button, "clicked", G_CALLBACK(on_solve), &interface);
    g_signal_connect(start, "clicked", G_CALLBACK(on_start), &interface);
    g_signal_connect(restart, "clicked", G_CALLBACK(on_restart), &interface);

    gtk_widget_show(GTK_WIDGET(window));

    gtk_main();

    return 0;
}
