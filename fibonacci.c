#include "fibonacci.h"

#include <stdio.h>

// Function to calculate the nth Fibonacci number using iteration
unsigned long long calculate_fibonacci(int nth) {
    unsigned long long first  = 0;
    unsigned long long second = 1;
    unsigned long long sum    = 0;

    for (int ix = 0; ix < nth; ix++) {
        sum    = first + second;
        first  = second;
        second = sum;
    }
    return first;
}
