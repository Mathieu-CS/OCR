#ifndef HELPERS_H_
#define HELPERS_H_
int **hough_transform(Image *in, Image *clean, int_list *edges_x,
    int_list *edges_y, bool verbose_mode, char *verbose_path);
#endif
