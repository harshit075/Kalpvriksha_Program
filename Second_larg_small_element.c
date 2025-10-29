#include <stdio.h>
#include <limits.h>

int main() {
    int size;
    printf("Enter number of elements: ");
    scanf("%d", &size);

    if (size < 2) {
        printf("Array must have at least 2 elements.\n");
        return 0;
    }

    int arr[size];
    printf("Enter %d elements: ", size);
    for (int i = 0; i < size; i++)
        scanf("%d", &arr[i]);

    int smallest = INT_MAX, secondSmallest = INT_MAX;
    int largest = INT_MIN, secondLargest = INT_MIN;

    for (int i = 0; i < size; i++) {
        int num = arr[i];

       
        if (num < smallest) {
            secondSmallest = smallest;
            smallest = num;
        } else if (num > smallest && num < secondSmallest) {
            secondSmallest = num;
        }

       
        if (num > largest) {
            secondLargest = largest;
            largest = num;
        } else if (num < largest && num > secondLargest) {
            secondLargest = num;
        }
    }

    if (smallest == secondSmallest || largest == secondLargest)
        printf("All elements are the same or insufficient distinct elements.\n");
    else
        printf("Second Smallest: %d\nSecond Largest: %d\n", secondSmallest, secondLargest);

    return 0;
}
