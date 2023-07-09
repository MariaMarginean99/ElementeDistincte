#include <stdio.h>
#include <sys/time.h>

void findDistinctElements(int arr[], int size) {
    // Iterate through each element of the array
    for (int i = 0; i < size; i++) {
        int j;
        // Check if the current element is already visited
        for (j = 0; j < i; j++) {
            if (arr[i] == arr[j])
                break;
        }
        // If the current element is not visited, print it
        // if (i == j) {
        //     printf("%d ", arr[i]);
        // }
    }
    // printf("\n");
}

int main() {
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    int arr[] = {1, 2, 3 , 1 , 3, 4 , 1 , 2, 5, 6};
    int size = sizeof(arr) / sizeof(arr[0]);

    // printf("Distinct elements in the array: ");
    //find distinct elements    
    findDistinctElements(arr, size);

    gettimeofday(&end_time, NULL);
    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
    
    printf("Elapsed time: %.5f seconds\n", elapsed_time);
    
    return 0;
}
