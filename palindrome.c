#include <stdio.h>

int main() {
    int arraySize;
    printf("Enter number of elements: ");
    scanf("%d", &arraySize);

    int arr[arraySize];
    printf("Enter %d elements: ", arraySize);
    for (int currentIndex = 0; currentIndex < arraySize; currentIndex++)
        scanf("%d", &arr[currentIndex]);

    int *leftPointer = arr;
    int *rightPointer = arr + arraySize - 1;
    int isPalindrome = 1;

    while (leftPointer < rightPointer) {
        if (*leftPointer != *rightPointer) {
            isPalindrome = 0;
            break;
        }
        leftPointer++;
        rightPointer--;
    }

    if (isPalindrome)
        printf("Palindrome\n");
    else
        printf("Not a palindrome\n");

    return 0;
}
