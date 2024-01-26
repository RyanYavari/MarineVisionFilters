#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "netpbm.h"
#include <stdio.h>
#include <stdbool.h>
#include <gaussian.c>
#include <convolve.c>
#include "gaussian.c"
#include "convolve.c"

Matrix smoothing_filter(Matrix m1, Matrix m2);
Matrix convolve(Matrix m1, Matrix m2);

Image canny(Image img)
{

    Matrix imgMatrix = image2Matrix(img);

    double smoothFilterArray[3][3] = {
        {1.0, 1.0, 1.0},
        {1.0, 1.0, 1.0},
        {1.0, 1.0, 1.0},
    };

    Matrix smoothFilterMatrix = createMatrixFromArray((double *)smoothFilterArray, 3, 3);
    Matrix smoothMatrix = smoothing_filter(imgMatrix, smoothFilterMatrix);

    double PArray[2][2] = {
        {0.7, 0.7},
        {-0.7, -0.7},
    };

    double QArray[2][2] = {
        {0.7, -0.7},
        {0.7, -0.7},
    };

    Matrix Pmatrix = createMatrixFromArray((double *)PArray, 2, 2);
    Matrix Qmatrix = createMatrixFromArray((double *)QArray, 2, 2);

    Matrix P = convolve(smoothMatrix, Pmatrix);
    Matrix Q = convolve(smoothMatrix, Qmatrix);

    Matrix m = createMatrix(img.height, img.width);
    Matrix a = createMatrix(img.height, img.width);

    for (int x = 1; x < img.width - 1; x++)
    {
        for (int y = 1; y < img.height - 1; y++)
        {
            int Pij = P.map[y][x];
            int Qij = Q.map[y][x];

            m.map[y][x] = sqrt(Pij * Pij + Qij * Qij);
            a.map[y][x] = atan2(Qij, Pij);
        }
    }

    Matrix C = createMatrix(img.height, img.width);

    for (int x = 1; x < img.width - 1; x++)
    {
        for (int y = 1; y < img.height - 1; y++)
        {
            if ((a.map[y][x] < 22.5 || a.map[y][x] > 337.5) || (a.map[y][x] > 157.5 && a.map[y][x] < 202.5))
            {
                C.map[y][x] = 0;
            }
            else if ((a.map[y][x] > 22.5 && a.map[y][x] < 67.5) || (a.map[y][x] > 202.5 && a.map[y][x] < 247.5))
            {
                C.map[y][x] = 1;
            }
            else if ((a.map[y][x] > 67.5 && a.map[y][x] < 112.5) || (a.map[y][x] > 247.5 && a.map[y][x] < 292.5))
            {
                C.map[y][x] = 2;
            }
            else
            {
                C.map[y][x] = 3;
            }
        }
    }

    Matrix E = createMatrix(img.height, img.width);

    for (int x = 2; x < img.width - 2; x++)
    {
        for (int y = 2; y < img.height - 2; y++)
        {
            if (C.map[y][x] == 0)
            {
                if (m.map[y][x] < m.map[y - 1][x] || m.map[y][x] < m.map[y + 1][x])
                {
                    E.map[y][x] = 0;
                    E.map[y][x] = 0;
                }
                else
                {
                    E.map[y][x] = m.map[y][x];
                }
            }
            if (C.map[y][x] == 1)
            {
                if (m.map[y][x] < m.map[y + 1][x - 1] || m.map[y][x] < m.map[y - 1][x + 1])
                {
                    E.map[y][x] = 0;
                }
                else
                {
                    E.map[y][x] = m.map[y][x];
                }
            }
            if (C.map[y][x] == 2)
            {
                if (m.map[y][x] < m.map[y][x - 1] || m.map[y][x] < m.map[y][x + 1])
                {
                    E.map[y][x] = 0;
                }
                else
                {
                    E.map[y][x] = m.map[y][x];
                }
            }
            if (C.map[y][x] == 3)
            {
                if (m.map[y][x] < m.map[y - 1][x - 1] || m.map[y][x] < m.map[y + 1][x + 1])
                {
                    E.map[y][x] = 0;
                }
                else
                {
                    E.map[y][x] = m.map[y][x];
                }
            }
        }
    }

    int high_threshold = 25;
    int low_threshold = 4;

    // 2 is edge, 1 is candidate, 0 is not an edge
    Matrix candidateMat = createMatrix(img.height, img.width);

    for (int x = 2; x < img.width - 2; x++)
    {
        for (int y = 2; y < img.height - 2; y++)
        {
            if (E.map[y][x] > high_threshold)
            {
                candidateMat.map[y][x] = 2;
            }
            else if (E.map[y][x] > low_threshold)
            {
                candidateMat.map[y][x] = 1;
            }
        }
    }

    Matrix finalMatrix = createMatrix(img.height, img.width);
    Matrix visited = createMatrix(img.height, img.width);

    // printMatrix(candidateMat);

    for (int x = 2; x < img.width - 2; x++)
    {
        for (int y = 2; y < img.height - 2; y++)
        {
            if (cannyLinking(candidateMat, x, y, visited))
            {
                finalMatrix.map[y][x] = 255;
            }
        }
    }

    Image cannyImage = matrix2Image(finalMatrix, 0, 1);
    return cannyImage;
}
