#include <stdio.h>
#include <stdlib.h>

// Function to calculate factorial
unsigned long long factorial(int n) {
    if (n == 0 || n == 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

int main(int argc, char *argv[]) {
    // Check if input is provided
    if (argc != 2) {
        printf("Usage: %s <number>\n", argv[0]);
        return 1;
    }

    // Convert command-line argument to integer
    int num = atoi(argv[1]);

    // Validate input
    if (num < 0) {
        printf("Error: Factorial is not defined for negative numbers.\n");
        return 1;
    }

    // Calculate factorial
    unsigned long long result = factorial(num);

    // Print result
    printf("Factorial of %d is %llu\n", num, result);
    return 0;
}
