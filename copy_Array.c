#include <stdio.h>

int main() {
    int numberOfElements;
    printf("Enter number of elements: ");
    scanf("%d", &numberOfElements);

    int sourceArray[numberOfElements], destinationArray[numberOfElements];

    printf("Enter %d elements for array A: ", numberOfElements);
    for (int elementIndex = 0; elementIndex < numberOfElements; elementIndex++)
        scanf("%d", &sourceArray[elementIndex]);

    int *pointerSourceArray = sourceArray;
    int *pointerDestinationArray = destinationArray;

    for (int copyIndex = 0; copyIndex < numberOfElements; copyIndex++)
        *(pointerDestinationArray + copyIndex) = *(pointerSourceArray + copyIndex);

    printf("Array A: ");
    for (int printIndex = 0; printIndex < numberOfElements; printIndex++)
        printf("%d ", *(sourceArray + printIndex));

    printf("\nArray B: ");
    for (int printIndex = 0; printIndex < numberOfElements; printIndex++)
        printf("%d ", *(destinationArray + printIndex));

    printf("\n");

    return 0;
}
