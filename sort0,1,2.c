#include <stdio.h>

void sortColors(int arr[], int n) {
    int low = 0, mid = 0, high = n - 1;

    while (mid <= high) {
        if (arr[mid] == 0) {
            int temp = arr[mid];
            arr[mid] = arr[low];
            arr[low] = temp;
            low++;
            mid++;
        }
        else if (arr[mid] == 1)
            mid++;
        else {
            int temp = arr[mid];
            arr[mid] = arr[high];
            arr[high] = temp;
            high--;
        }
    }
}

int main() {
    int arr[] = {2,0,2,1,1,0};
    int n = 6;

    sortColors(arr, n);

    for(int i=0;i<n;i++)
        printf("%d ", arr[i]);
}
