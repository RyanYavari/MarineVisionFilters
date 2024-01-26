#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "netpbm.h"
#include <stdio.h>
#include <stdbool.h>
#include "gaussian.c"
#include "convolve.c"

Matrix smoothing_filter(Matrix m1, Matrix m2);
Matrix convolve(Matrix m1, Matrix m2);

Image sobel(Image img)
{

    Matrix imgMatrix = image2Matrix(img);

    double smoothFilterArray[3][3] = {
        {1.0, 1.0, 1.0},
        {1.0, 1.0, 1.0},
        {1.0, 1.0, 1.0},
    };

    Matrix smoothFilterMatrix = createMatrixFromArray((double *)smoothFilterArray, 3, 3);
    Matrix smoothMatrix = smoothing_filter(imgMatrix, smoothFilterMatrix);

    double SiArray[3][3] = {
        {4.0, 8.0, 4.0},
        {0.0, 0.0, 0.0},
        {-4.0, -8.0, -4.0},
    };

    double SjArray[3][3] = {
        {4.0, 0.0, -4.0},
        {8.0, 0.0, -8.0},
        {4.0, 0.0, -4.0},
    };

    Matrix SiMatrix = createMatrixFromArray((double *)SiArray, 3, 3);
    Matrix SjMatrix = createMatrixFromArray((double *)SjArray, 3, 3);

    Matrix Si = convolve(smoothMatrix, SiMatrix);
    Matrix Sj = convolve(smoothMatrix, SjMatrix);

    Matrix mat = createMatrix(img.height, img.width);

    for (int x = 1; x < img.width - 1; x++)
    {
        for (int y = 1; y < img.height - 1; y++)
        {
            int s1 = Si.map[y][x];
            int s2 = Sj.map[y][x];
            mat.map[y][x] = sqrt(s1 * s1 + s2 * s2) / 3;
        }
    }

    Image sobelImage = matrix2Image(mat, 0, 1);
    return sobelImage;
}