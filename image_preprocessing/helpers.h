#ifndef HELPERS_H_
#define HELPERS_H_
#include "image.h"

void convolution(
    double *kernel, int ksize, Image *image, Image *out, bool normalize);
void convolution_mat(
    double *kernel, int ksize_x, int ksize_y, Image *in, int *out);
void _mkdir(const char *dir);
double clamp(double d, double min, double max);
void verbose_save(
    bool verbose_mode, char *verbose_path, char *file_name, Image *image);
int min_arr_index(int array[], int size);
double *spread_arr(int size, double min, double max, double step);
double degrees_to_rad(double degrees);

#endif