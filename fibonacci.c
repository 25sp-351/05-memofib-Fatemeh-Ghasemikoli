#include "fibonacci.h"
#include <gmp.h>

void calculate_fibonacci(int nth, mpz_t result) {
    mpz_t first, second;
    mpz_init_set_ui(first, 0);
    mpz_init_set_ui(second, 1);
    mpz_init(result);

    if (nth == 0) {
        mpz_set(result, first);
        return;
    }
    if (nth == 1) {
        mpz_set(result, second);
        return;
    }

// Calculate the nth Fibonacci number
    for (int ix = 2; ix <= nth; ix++) {
        mpz_add(result, first, second);
        mpz_set(first, second);
        mpz_set(second, result);
    }

    mpz_set(result, second); // Ensures correct nth Fibonacci number is returned

    mpz_clear(first);
    mpz_clear(second);
}
