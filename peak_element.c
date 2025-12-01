#include <stdio.h>

int findPeakElement(int nums[], int n) {
    int low = 0, high = n - 1;

    while (low < high) {
        int mid = (low + high) / 2;

        if (nums[mid] < nums[mid + 1])
            low = mid + 1;
        else
            high = mid;
    }

    return low;
}

int main() {
    int nums[] = {1,2,1,3,5,6,4};
    int n = 7;

    printf("%d\n", findPeakElement(nums, n));
}
