#include <stdio.h>

int main() {
    int numberOfElements;
    printf("Enter number of elements: ");
    scanf("%d", &numberOfElements);

    int array[numberOfElements];
    printf("Enter %d elements: ", numberOfElements);
    for (int elementIndex = 0; elementIndex < numberOfElements; elementIndex++)
        scanf("%d", &array[elementIndex]);

    int *arrayPointer = array;
    int uniqueElementCount = 0;

    for (int currentIndex = 0; currentIndex < numberOfElements; currentIndex++) {
        int isDuplicate = 0;
        for (int comparisonIndex = 0; comparisonIndex < uniqueElementCount; comparisonIndex++) {
            if (*(array + currentIndex) == *(array + comparisonIndex)) {
                isDuplicate = 1;
                break;
            }
        }
        if (!isDuplicate) {
            *(array + uniqueElementCount) = *(array + currentIndex);
            uniqueElementCount++;
        }
    }

    printf("Array after removing duplicates:\n");
    for (int printIndex = 0; printIndex < uniqueElementCount; printIndex++)
        printf("%d ", *(array + printIndex));

    printf("\nNew size: %d\n", uniqueElementCount);

    return 0;
}
