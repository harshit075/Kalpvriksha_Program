#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void matrixGenerator(int **matrix, int matrixSize) {
    for (int row = 0; row < matrixSize; row++) {
        for (int column = 0; column < matrixSize; column++) {
            *(*(matrix + row) + column) = rand() % 256;
        }
    }
}

void displayMatrix(int **matrix, int matrixSize) {
    for (int row = 0; row < matrixSize; row++) {
        for (int column = 0; column < matrixSize; column++) {
            printf("%4d ", *(*(matrix + row) + column));
        }
        printf("\n");
    }
}

void rotate_ByClockwise(int **matrix, int matrixSize) {
    for (int layer = 0; layer < matrixSize / 2; layer++) {
        int first = layer;
        int last = matrixSize - 1 - layer;

        for (int i = first; i < last; i++) {
            int offset = i - first;

            int *top    = *(matrix + first) + i;
            int *left   = *(matrix + (last - offset)) + first;
            int *bottom = *(matrix + last) + (last - offset);
            int *right  = *(matrix + i) + last;

            
            int temp = *left;
            *left = *bottom;
            *bottom = *right;
            *right = *top;
            *top = temp;
        }
    }
}

void smoothMatrix(int **matrix, int matrixSize) {
    int *temporaryRow = (int *)malloc(matrixSize * sizeof(int));
    if (temporaryRow == NULL) {
        printf("Memory allocation failed for temporaryRow.\n");
        exit(1);
    }

    for (int row = 0; row < matrixSize; row++) {
        for (int column = 0; column < matrixSize; column++) {
            int sum = 0, count = 0;

           
            for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
                for (int deltaColumn = -1; deltaColumn <= 1; deltaColumn++) {
                    int neighborRow = row + deltaRow;
                    int neighborColumn = column + deltaColumn;

                    
                    if (neighborRow >= 0 && neighborRow < matrixSize &&
                        neighborColumn >= 0 && neighborColumn < matrixSize) {
                        sum += matrix[neighborRow][neighborColumn];
                        count++;
                    }
                }
            }

            temporaryRow[column] = sum / count; 
        }

       
        for (int column = 0; column < matrixSize; column++) {
            matrix[row][column] = temporaryRow[column];
        }
    }

    free(temporaryRow);
}


int main() {
    int matrixSize;
    printf("Please enter matrix size 2-10 : ");
    scanf("%d", &matrixSize);

    if (matrixSize < 2 || matrixSize > 10) {
        printf("ERROR: Invalid matrix size (2–10 allowed).\n");
        return 0;
    }

    int **matrix = (int **)malloc(matrixSize * sizeof(int *));
    for (int row = 0; row < matrixSize; row++) {
        *(matrix + row) = (int *)malloc(matrixSize * sizeof(int));
    }

    srand(time(NULL));

    printf("\nOriginal randomly generated Matrix:\n");
    matrixGenerator(matrix, matrixSize);
    displayMatrix(matrix, matrixSize);

    printf("\n******************************************");

    rotate_ByClockwise(matrix, matrixSize);
    printf("\nMatrix after Clockwise Rotation:\n");
    displayMatrix(matrix, matrixSize);

    printf("\n*********************************************");

    smoothMatrix(matrix, matrixSize);
    printf("\nMatrix after applying 3x3 Smoothing Filter:\n");
    displayMatrix(matrix, matrixSize);

    for (int row = 0; row < matrixSize; row++)
        free(*(matrix + row));
    free(matrix);

    return 0;
}
