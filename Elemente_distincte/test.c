#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define VECTOR_SIZE 50000
#define UNIQUE_NUMBERS_COUNT 30
#define REPEAT_COUNT 4

int main() {
    int vector[VECTOR_SIZE];
    int unique_numbers[UNIQUE_NUMBERS_COUNT];
    int repeated_numbers[UNIQUE_NUMBERS_COUNT * REPEAT_COUNT];
    int remaining_numbers[VECTOR_SIZE - UNIQUE_NUMBERS_COUNT * REPEAT_COUNT];

    srand(time(NULL));

    // Generăm cel puțin 30 de numere unice
    int i;
    for (i = 0; i < UNIQUE_NUMBERS_COUNT; i++) {
        unique_numbers[i] = (rand() % 10000) + 1;
    }

    // Generăm celelalte numere în funcție de numerele unice
    int j, k = 0;
    for (i = 0; i < UNIQUE_NUMBERS_COUNT; i++) {
        for (j = 0; j < REPEAT_COUNT; j++) {
            repeated_numbers[k] = unique_numbers[i];
            k++;
        }
    }

    // Generăm restul numerelelor în mod aleatoriu
    for (i = 0; i < VECTOR_SIZE - UNIQUE_NUMBERS_COUNT * REPEAT_COUNT; i++) {
        remaining_numbers[i] = (rand() % 10000) + 1;
    }

    // Combinația finală a numerelor
    int l = 0;
    for (i = 0; i < UNIQUE_NUMBERS_COUNT * REPEAT_COUNT; i++) {
        vector[l] = repeated_numbers[i];
        l++;
    }
    for (i = 0; i < VECTOR_SIZE - UNIQUE_NUMBERS_COUNT * REPEAT_COUNT; i++) {
        vector[l] = remaining_numbers[i];
        l++;
    }

    // Amestecăm vectorul în mod aleatoriu
    for (i = VECTOR_SIZE - 1; i > 0; i--) {
        j = rand() % (i + 1);
        int temp = vector[i];
        vector[i] = vector[j];
        vector[j] = temp;
    }

    // Afișăm vectorul generat și numărul de apariții pentru fiecare număr unic
    FILE *fptr;
   fptr = fopen("test.txt","w");
    for (i = 0; i < VECTOR_SIZE; i++) {
        fprintf(fptr,"%d, ", vector[i]);
    }
    printf("\n");
    fclose(fptr); 
    return 0;
}