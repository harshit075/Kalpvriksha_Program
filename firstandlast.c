#include <stdio.h>

int firstPos(int arr[], int n, int target) {
    int low = 0, high = n - 1, ans = -1;

    while (low <= high) {
        int mid = (low + high) / 2;

        if (arr[mid] == target) {
            ans = mid;
            high = mid - 1;
        } 
        else if (arr[mid] < target)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return ans;
}

int lastPos(int arr[], int n, int target) {
    int low = 0, high = n - 1, ans = -1;

    while (low <= high) {
        int mid = (low + high) / 2;

        if (arr[mid] == target) {
            ans = mid;
            low = mid + 1;
        } 
        else if (arr[mid] < target)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return ans;
}

int main() {
    int arr[] = {2,4,4,4,7,8,8,10};
    int n = 8;
    int target = 4;

    int first = firstPos(arr, n, target);
    int last = lastPos(arr, n, target);

    printf("[%d, %d]\n", first, last);
    return 0;
}
