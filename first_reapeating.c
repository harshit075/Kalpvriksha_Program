#include <stdio.h>

int firstRepeating(int arr[], int n) {
    int freq[100000] = {0};
    int index[100000];

    for (int i = 0; i < 100000; i++)
        index[i] = -1;

    int result = -1;

    for (int i = 0; i < n; i++) {
        if (freq[arr[i]] == 0) {
            freq[arr[i]] = 1;
            index[arr[i]] = i;
        } else {
            if (result == -1 || index[arr[i]] < result)
                result = index[arr[i]];
        }
    }

    return result;
}

int main() {
    int arr[] = {4,5,2,4,3,2};
    int n = 6;

    printf("%d\n", firstRepeating(arr, n));  // Output: 0
    return 0;
}
