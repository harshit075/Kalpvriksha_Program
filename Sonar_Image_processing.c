#include <stdio.h>
#include <stdlib.h>
#include<time.h>

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

void rotate_Byclockwise(int **matrix, int matrixSize) {
    for (int layer = 0; layer < matrixSize / 2; layer++) {
        int first = layer;
        int last = matrixSize - 1 - layer;

        for (int row = first; row < last; row++) {
            int offset = row - first;

            int *top = *(matrix + first) + row;
            int *left = *(matrix + (last - offset)) + first;
            int *bottom = *(matrix + last) + (last - offset);
            int *right = *(matrix + row) + last;

            int temp = *top;
            *top = *left;
            *left = *bottom;
            *bottom = *right;
            *right = temp;
        }
    }
}

void smoothMatrix(int **matrix, int matrixSize) {
    int **tempMatrix = (int **)malloc(matrixSize * sizeof(int *));
    for (int i = 0; i < matrixSize; i++)
        tempMatrix[i] = (int *)malloc(matrixSize * sizeof(int));

 
    for (int row = 0; row < matrixSize; row++) {
        for (int column = 0; column < matrixSize; column++) {

            int sum = 0, count = 0;

            sum += matrix[row][column];
            count++;

            // top neighbor
            if (row - 1 >= 0) {
                sum += matrix[row - 1][column];
                count++;
            }

            // bottom neighbor
            if (row + 1 < matrixSize) {
                sum += matrix[row + 1][column];
                count++;
            }

            // left neighbor
            if (column - 1 >= 0) {
                sum += matrix[row][column - 1];
                count++;
            }

            // right neighbor
            if (column + 1 < matrixSize) {
                sum += matrix[row][column + 1];
                count++;
            }

            // top left
            if (row - 1 >= 0 && column - 1 >= 0) {
                sum += matrix[row - 1][column - 1];
                count++;
            }

            // top right
            if (row - 1 >= 0 && column + 1 < matrixSize) {
                sum += matrix[row - 1][column + 1];
                count++;
            }

            // bottom left
            if (row + 1 < matrixSize && column - 1 >= 0) {
                sum += matrix[row + 1][column - 1];
                count++;
            }

            // bottom right
            if (row + 1 < matrixSize && column + 1 < matrixSize) {
                sum += matrix[row + 1][column + 1];
                count++;
            }

            
            tempMatrix[row][column] = sum / count;
        }
    }

    for (int row = 0; row < matrixSize; row++) {
        for (int column = 0; column < matrixSize; column++) {
            matrix[row][column] = tempMatrix[row][column];
        }
    }

    
    for (int i = 0; i < matrixSize; i++)
        free(tempMatrix[i]);
    free(tempMatrix);
}

int main() {
    int matrixSize;
    printf("Please enter matrix size (2-10): ");
    scanf("%d", &matrixSize);

    if (matrixSize < 2 || matrixSize > 10) {
        printf("ERROR: Invalid size of matrix.pls enter between 2 and 10.\n");
        return 0;
    }

    int **matrix = (int **)malloc(matrixSize * sizeof(int *));
    for (int row = 0; row < matrixSize; row++)
        *(matrix + row) = (int *)malloc(matrixSize * sizeof(int));

    srand(time(NULL));

    printf("\nOriginal Randomly Generated Matrix:\n");
    matrixGenerator(matrix, matrixSize);
    displayMatrix(matrix, matrixSize);
    printf("\n******************************************");

    rotate_Byclockwise(matrix, matrixSize);
    printf("\nMatrix after Clockwise Rotation:\n");
    displayMatrix(matrix, matrixSize);

    printf("\n*********************************************");

    smoothMatrix(matrix, matrixSize);
    printf("\nMatrix after Applying 3x3 Smoothing Filter:\n");
    displayMatrix(matrix, matrixSize);

    for (int row = 0; row < matrixSize; row++)
        free(*(matrix + row));
    free(matrix);

    return 0;
}
