#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "netpbm.h"
#include <stdio.h>
#include <stdbool.h>

Matrix smoothing_filter(Matrix m1, Matrix m2);
Matrix convolve(Matrix m1, Matrix m2);

Matrix smoothing_filter(Matrix m1, Matrix m2)
{
    Matrix mat = createMatrix(m1.height, m1.width);

    bool evenHeight = (m2.height % 2 == 0);
    bool evenWidth = m2.width % 2 == 0;

    int startCol = evenWidth ? m2.width / 2 - 1 : floor(m2.width / 2);
    int startRow = evenHeight ? m2.height / 2 - 1 : floor(m2.height / 2);
    //    printf("startRow: %d ", startRow);
    //    printf("\n");

    int endCol = evenWidth ? m1.width - startCol - 1 : m1.width - startCol;
    int endRow = evenHeight ? m1.height - startRow - 1 : m1.height - startRow;

    for (int x = startCol; x < endCol; x++)
    {
        for (int y = startRow; y < endRow; y++)
        {
            int newValue = 0;
            int filterSize = m2.width * m2.height;

            for (int i = 0; i < m2.width; i++)
            {
                for (int j = 0; j < m2.height; j++)
                {
                    int yCoord = evenHeight ? y - m2.height / 2 + j + 1 : y - m2.height / 2 + j;
                    int xCoord = evenWidth ? x - m2.width / 2 + i + 1 : x - m2.width / 2 + i;
                    int originalValue = m1.map[yCoord][xCoord];
                    //                    printf("originalValue: %d ", originalValue);
                    //                    printf("\n");

                    int weightedValue = originalValue * m2.map[j][i] / filterSize;
                    //                    printf("weightedValue: %d ", weightedValue);
                    //                    printf("\n");

                    newValue += weightedValue;
                }
            }

            //            printf("Y: %d ", y);
            //            printf("\n");

            mat.map[y][x] = newValue;
        }
    }
    // printMatrix(mat);
    return mat;
}