#include <stdio.h>
#include <stdlib.h>

void findClosestElements(int arr[], int n, int k, int x) {
    int left = 0, right = n - 1;

    while (right - left + 1 > k) {
        if (x - arr[left] <= arr[right] - x)
            right--;
        else
            left++;
    }

    for (int i = left; i <= right; i++) {
        printf("%d ", arr[i]);
    }
}

int main() {
    int arr[] = {1,2,3,4,5};
    int n = 5, k = 4, x = 3;

    findClosestElements(arr, n, k, x);
    return 0;
}
