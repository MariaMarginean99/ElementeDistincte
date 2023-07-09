#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <windows.h>

// Structure to hold arguments for each thread
typedef struct {
    int* arr;
    int start;
    int end;
} ThreadArgs;

struct timeval start_time, end_time;

DWORD WINAPI findDistinctElements(LPVOID arg) {
    ThreadArgs* threadArgs = (ThreadArgs*)arg;
    int* arr = threadArgs->arr;
    int start = threadArgs->start;
    int end = threadArgs->end;

    // Iterate through each element of the array
    for (int i = start; i < end; i++) {
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

    return 0;
}

int main() {

    gettimeofday(&start_time, NULL);

    int arr[] = {1, 2, 3 , 1 , 3, 4 , 1 , 2, 5, 6};
    int size = sizeof(arr) / sizeof(arr[0]);

    // printf("Distinct elements in the array: ");

    int numThreads = 4; // Number of threads to use
    HANDLE threads[numThreads];
    ThreadArgs threadArgs[numThreads];

    int elementsPerThread = size / numThreads;
    int remainder = size % numThreads;

    int start = 0;
    int end;

    // Create threads and assign work to each thread
    for (int i = 0; i < numThreads; i++) {
        end = start + elementsPerThread + (i < remainder ? 1 : 0);

        threadArgs[i].arr = arr;
        threadArgs[i].start = start;
        threadArgs[i].end = end;

        threads[i] = CreateThread(NULL, 0, findDistinctElements, &threadArgs[i], 0, NULL);

        start = end;
    }

    // Wait for all threads to finish
    WaitForMultipleObjects(numThreads, threads, TRUE, INFINITE);

    printf("\n");
    gettimeofday(&end_time, NULL);
    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
    printf("Elapsed time: %.6f seconds\n", elapsed_time);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <sys/time.h>

void displayDistinctElements(int arr[], int size, int rank, int num_procs) {
    // printf("Distinct elements in process %d: ", rank);

    // Traverse the array
    for (int i = 0; i < size; i++) {
        // Check if the current element is already printed locally
        int j;
        for (j = 0; j < i; j++) {
            if (arr[i] == arr[j]) {
                break;
            }
        }

        // If the current element is distinct locally, print it
        // if (i == j) {
        //     printf("%d ", arr[i]);
        // }
    }

    // printf("\n");

    // Send distinct elements to the root process (process 0)
    if (rank != 0) {
        MPI_Send(arr, size, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else {
        // Receive distinct elements from other processes and print them
        for (int p = 1; p < num_procs; p++) {
            MPI_Status status;
            MPI_Probe(p, 0, MPI_COMM_WORLD, &status);

            int recv_size;
            MPI_Get_count(&status, MPI_INT, &recv_size);

            int* recv_arr = (int*)malloc(recv_size * sizeof(int));

            MPI_Recv(recv_arr, recv_size, MPI_INT, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // printf("Distinct elements received from process %d: ", p);
            // for (int i = 0; i < recv_size; i++) {
            //     printf("%d ", recv_arr[i]);
            // }
            // printf("\n");

            free(recv_arr);
        }
    }
}

int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);

    int rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int arr[] = {480, 64, 870, 211, 509, 480, 989, 33, 127, 867, 480, 706, 155, 722, 258, 456, 883, 578, 865, 24, 988, 834, 883, 691, 979, 360, 671, 288, 737, 387, 614, 873, 628, 497, 68, 338, 54, 832, 126, 390, 64, 833, 535, 72, 476, 899, 141, 380, 89, 170, 669, 282, 662, 383, 176, 504, 274, 294, 197, 455, 738, 731, 958, 195, 61, 693, 537, 72, 681, 38, 367, 977, 61, 128, 850, 64, 919, 799, 505, 350, 121, 244, 174, 844, 196, 476, 740, 623, 905, 614, 618, 414, 214, 705, 777, 831, 64, 257, 472, 169, 152, 205, 262, 156, 770, 499, 647, 184, 933, 558, 293, 654, 252, 755, 654, 51, 680, 542, 474, 612, 193, 856, 783, 192, 288, 774, 724, 747, 289, 181, 491, 857, 48, 125, 647, 530, 940, 956, 367, 621, 88, 850, 126, 220, 974, 90, 738, 948, 424, 137, 28, 46, 477, 117, 168, 462, 858, 187, 896, 58, 68, 866, 367, 915, 698, 998, 636, 249, 29, 740, 741, 898, 827, 907, 88, 680, 63, 510, 523, 524, 287, 585, 309, 588, 734, 347, 457, 605, 76, 60, 866, 930, 852, 193, 394, 162, 257, 912, 184, 460, 25, 509, 764, 334, 321, 706, 498, 754, 596, 804, 621, 288, 636, 671, 680, 124, 167, 395, 866, 256, 25, 808, 396, 996, 778, 453, 384, 857, 17, 677, 110, 684, 46, 469, 191, 251, 193, 891, 179, 633, 298, 762, 81, 107, 31, 156, 663, 726, 202, 567, 672, 547, 970, 431, 487, 648, 598, 699, 219, 419, 904, 680, 878, 248, 244, 328, 425, 541, 64, 519, 777, 938, 429, 134, 480, 495, 285, 948, 725, 252, 930, 286, 356, 641, 249, 505, 976, 638, 43, 98, 260, 572, 650, 409, 747, 48, 82, 92, 308, 451, 92, 879, 93, 303, 642, 140, 87, 227, 618, 838, 852, 752, 88, 829, 782, 777, 560, 862, 359, 972, 270, 177, 27, 828, 446, 336, 616, 86, 618, 348, 614, 102, 466, 828, 736, 332, 816, 380, 974, 621, 183, 192, 567, 489, 292, 610, 93, 89, 419, 978, 944, 437, 340, 372, 360, 937, 904, 979, 125, 26, 990, 816, 894, 350, 529, 102, 596, 848, 721, 685, 248, 336, 20, 982, 915, 24, 445, 442, 694, 733, 193, 970, 359, 812, 553, 193, 8, 234, 860, 491, 971, 971, 165, 831, 881, 623, 719, 686, 452, 635, 17, 322, 199, 2, 813, 223, 294, 420, 236, 578, 297, 904, 47, 714, 238, 713, 567, 63, 402, 894, 860, 587, 187, 992, 576, 876, 888, 253, 808, 176, 470, 57, 171, 64, 157, 351, 390, 682, 209, 216, 64, 585, 92, 204, 686, 214, 235, 706, 829, 329, 803, 693, 519, 503, 982, 347, 413, 559, 102, 772, 804, 258, 708, 175, 206, 752, 550, 246, 500, 336, 845, 707, 903, 152, 211, 101, 299, 150, 908, 158, 621, 671, 737, 28, 64, 99, 240, 492, 631, 751, 288, 286, 782, 783, 489, 768, 333, 197, 225, 957};
    int size = sizeof(arr) / sizeof(arr[0]);

    // Calculate the number of elements to process in each process
    int local_size = size / num_procs;
    int remaining = size % num_procs;

    // Adjust the local size for the last process
    if (rank == num_procs - 1) {
        local_size += remaining;
    }

    // Allocate memory for the local array
    int* local_arr = (int*)malloc(local_size * sizeof(int));

    // Scatter the array to all processes
    MPI_Scatter(arr, local_size, MPI_INT, local_arr, local_size, MPI_INT, 0, MPI_COMM_WORLD);
    struct timeval start_time, end_time;

    if (rank == 0) {
        gettimeofday(&start_time, NULL);
    }
    // Display distinct elements in each process
    displayDistinctElements(local_arr, local_size, rank, num_procs);

    // Gather distinct elements from all processes
    if (rank == 0) {
        int* recv_sizes = (int*)malloc(num_procs * sizeof(int));
        int total_size = local_size;

        recv_sizes[0] = local_size;
        for (int p = 1; p < num_procs; p++) {
            MPI_Recv(&recv_sizes[p], 1, MPI_INT, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_size += recv_sizes[p];
        }

        int* distinct_elements = (int*)malloc(total_size * sizeof(int));
        int offset = 0;

        // Copy distinct elements from the root process (process 0)
        for (int i = 0; i < local_size; i++) {
            distinct_elements[offset++] = local_arr[i];
        }

        // Receive distinct elements from other processes
        for (int p = 1; p < num_procs; p++) {
            MPI_Recv(distinct_elements + offset, recv_sizes[p], MPI_INT, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            offset += recv_sizes[p];
        }

        // Remove duplicate elements
        int* final_distinct_elements = (int*)malloc(total_size * sizeof(int));
        int final_size = 0;

        for (int i = 0; i < total_size; i++) {
            int j;
            for (j = 0; j < final_size; j++) {
                if (distinct_elements[i] == final_distinct_elements[j]) {
                    break;
                }
            }

            if (j == final_size) {
                final_distinct_elements[final_size++] = distinct_elements[i];
            }
        }

        // Print distinct elements gathered from all processes
        // printf("Distinct elements gathered from all processes: ");
        // for (int i = 0; i < final_size; i++) {
        //     printf("%d ", final_distinct_elements[i]);
        // }
        // printf("\n");

        free(recv_sizes);
        free(distinct_elements);
        free(final_distinct_elements);
    }
    else {
        // Send the size of distinct elements to the root process
        MPI_Send(&local_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        // Send distinct elements to the root process
        MPI_Send(local_arr, local_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    // Cleanup
    free(local_arr);
    MPI_Finalize();

    if (rank == 0) {
        gettimeofday(&end_time, NULL);
        double execution_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1e6;
        printf("Execution time: %.6f seconds\n", execution_time);
    }
    return 0;
}