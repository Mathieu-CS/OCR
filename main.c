#include "detect_rect.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    char *p = malloc(sizeof("image_01.bmp"));
    p = "image_01.bmp";

    detect_rect(p);
    return 0;
}