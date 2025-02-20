#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "cache.h"
#include "fibonacci.h"

#define TESTS_RUN 1000     // Number of Fibonacci tests to run
#define MAX_FIB_INDEX 90   // Maximum Fibonacci index to compute

// Function to generate a random number
int generate_random(int lower, int upper);

// Function to determine ordinal suffix (st, nd, rd, th)
const char *ordinal_suffix(int num);

int main(int argc, char *argv[]) {
    if (argc > 2) {
        fprintf(stderr, "Usage: %s [cache.so]\n", argv[0]);
        return 1;
    }

    ProviderFunction fibonacci_provider = calculate_fibonacci; // Default Fibonacci function
    bool using_cache = argc > 1; // Check if cache is being used
    Cache *cache_system = NULL;

    if (using_cache) {
        cache_system = load_cache_module(argv[1]); // Load cache module

        if (!cache_system) {
            fprintf(stderr, "Failed to load cache module\n");
            return 1;
        }
        fibonacci_provider = cache_system->set_provider_func(fibonacci_provider); // Set caching provider
    }

    for (int i = 0; i < TESTS_RUN; i++) {
        int rand_num = generate_random(1, MAX_FIB_INDEX); // Generate a random Fibonacci index

        printf("\n\nStarting test %2d: %d\n", i, rand_num);

        unsigned long long fib_result = fibonacci_provider(rand_num); // Compute Fibonacci number

        printf("Completed test %2d: %d%s Fibonacci number is %llu\n", i,
               rand_num, ordinal_suffix(rand_num), fib_result);

        if (cache_system != NULL && i == TESTS_RUN / 2) {
            printf("Taking a pause. Clearing cache statistics.\n");
            cache_system->reset_statistics(); // Reset cache statistics mid-way
        }
    }

    if (using_cache) {
        printf("\n\n");

        CacheStat *stats = cache_system->get_statistics(); // Retrieve cache statistics
        display_cache_statistics(fileno(stdout), stats);

        if (stats)
            free(stats);

        printf("\n\n");

        cache_system->cache_cleanup(); // Clean up cache before exiting
        free(cache_system);
    }
}

int generate_random(int lower, int upper) {
    return lower + arc4random_uniform(upper - lower); // Generate random number in range
}

const char *ordinal_suffix(int num) {
    int last_digit = num % 10;
    int last_two_digits = num % 100;

    if (last_two_digits >= 11 && last_two_digits <= 13)
        return "th";

    switch (last_digit) {
        case 1:
            return "st";
        case 2:
            return "nd";
        case 3:
            return "rd";
        default:
            return "th";
    }
}
