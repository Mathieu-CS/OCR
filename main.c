#include <err.h>
#include <stdio.h>
#include <math.h>

#include "operations.h"
#include "grayscale.h"
#include "Gauss.h"
#include "Sobel.h"

int main()
{
    grayscale("Images/image_06.jpeg");
    Gauss("grayscale.bmp");
    Sobel("Gauss.bmp");

    return 0;
}