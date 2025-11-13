#include <stdio.h>

int main() {
    int size;
    printf("Enter number of elements: ");
    scanf("%d", &size);

    int arr[size];
    printf("Enter %d elements: ", size);
    for (int currentIndex = 0; currentIndex < size; currentIndex++)
        scanf("%d", &arr[currentIndex]);

    int visited[size];
    for (int currentIndex = 0; currentIndex < size; currentIndex++)
        visited[currentIndex] = 0;

    for (int outerIndex = 0; outerIndex < size; outerIndex++) {
        if (visited[outerIndex])
            continue;

        int frequencyCount = 1;
        for (int innerIndex = outerIndex + 1; innerIndex < size; innerIndex++) {
            if (arr[outerIndex] == arr[innerIndex]) {
                frequencyCount++;
                visited[innerIndex] = 1;
            }
        }
        printf("Element %d appears %d times\n", arr[outerIndex], frequencyCount);
    }

    return 0;
}
